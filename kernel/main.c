
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                            main.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                                                    Forrest Yu, 2005
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#include "type.h"
#include "stdio.h"
#include "const.h"
#include "protect.h"
#include "string.h"
#include "fs.h"
#include "proc.h"
#include "tty.h"
#include "console.h"
#include "global.h"
#include "proto.h"


//用户数据
#define MAX_USER_COUNT 10

struct Users{
	char username[128];
	char password[128];
}users[MAX_USER_COUNT];
char currentUser[128] = "/";
int usercount = 0;
int currentUserIndex=-1;


//文件数据
char currentFolder[128] = "/";
char filepath[128] = "";
char files[20][128];
char userfiles[20][128];
int filequeue[50];
int filecount = 0;
int isEntered = 0;
//int UserSwitch = 0;
int leiflag = 0;


/*****************************************************************************
 *                               kernel_main
 *****************************************************************************/
/**
 * jmp from kernel.asm::_start. 
 * 
 *****************************************************************************/
PUBLIC int kernel_main()
{
	disp_str("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");

	int i, j, eflags, prio;
        u8  rpl;
        u8  priv; /* privilege */

	struct task * t;
	struct proc * p = proc_table;

	char * stk = task_stack + STACK_SIZE_TOTAL;

	for (i = 0; i < NR_TASKS + NR_PROCS; i++,p++,t++) {
		if (i >= NR_TASKS + NR_NATIVE_PROCS) {
			p->p_flags = FREE_SLOT;
			continue;
		}

	        if (i < NR_TASKS) {     /* TASK */
                        t	= task_table + i;
                        priv	= PRIVILEGE_TASK;
                        rpl     = RPL_TASK;
                        eflags  = 0x1202;/* IF=1, IOPL=1, bit 2 is always 1 */
			prio    = 15;
                }
                else {                  /* USER PROC */
                        t	= user_proc_table + (i - NR_TASKS);
                        priv	= PRIVILEGE_USER;
                        rpl     = RPL_USER;
                        eflags  = 0x202;	/* IF=1, bit 2 is always 1 */
			prio    = 5;
                }

		strcpy(p->name, t->name);	/* name of the process */
		p->p_parent = NO_TASK;

		if (strcmp(t->name, "INIT") != 0) {
			p->ldts[INDEX_LDT_C]  = gdt[SELECTOR_KERNEL_CS >> 3];
			p->ldts[INDEX_LDT_RW] = gdt[SELECTOR_KERNEL_DS >> 3];

			/* change the DPLs */
			p->ldts[INDEX_LDT_C].attr1  = DA_C   | priv << 5;
			p->ldts[INDEX_LDT_RW].attr1 = DA_DRW | priv << 5;
		}
		else {		/* INIT process */
			unsigned int k_base;
			unsigned int k_limit;
			int ret = get_kernel_map(&k_base, &k_limit);
			assert(ret == 0);
			init_desc(&p->ldts[INDEX_LDT_C],
				  0, /* bytes before the entry point
				      * are useless (wasted) for the
				      * INIT process, doesn't matter
				      */
				  (k_base + k_limit) >> LIMIT_4K_SHIFT,
				  DA_32 | DA_LIMIT_4K | DA_C | priv << 5);

			init_desc(&p->ldts[INDEX_LDT_RW],
				  0, /* bytes before the entry point
				      * are useless (wasted) for the
				      * INIT process, doesn't matter
				      */
				  (k_base + k_limit) >> LIMIT_4K_SHIFT,
				  DA_32 | DA_LIMIT_4K | DA_DRW | priv << 5);
		}

		p->regs.cs = INDEX_LDT_C << 3 |	SA_TIL | rpl;
		p->regs.ds =
			p->regs.es =
			p->regs.fs =
			p->regs.ss = INDEX_LDT_RW << 3 | SA_TIL | rpl;
		p->regs.gs = (SELECTOR_KERNEL_GS & SA_RPL_MASK) | rpl;
		p->regs.eip	= (u32)t->initial_eip;
		p->regs.esp	= (u32)stk;
		p->regs.eflags	= eflags;

		p->ticks = p->priority = prio;

		p->p_flags = 0;
		p->p_msg = 0;
		p->p_recvfrom = NO_TASK;
		p->p_sendto = NO_TASK;
		p->has_int_msg = 0;
		p->q_sending = 0;
		p->next_sending = 0;

		for (j = 0; j < NR_FILES; j++)
			p->filp[j] = 0;

		stk -= t->stacksize;
	}

	k_reenter = 0;
	ticks = 0;

	p_proc_ready	= proc_table;

	init_clock();
        init_keyboard();

	restart();

	while(1){}
}


/*****************************************************************************
 *                                get_ticks
 *****************************************************************************/
PUBLIC int get_ticks()
{
	MESSAGE msg;
	reset_msg(&msg);
	msg.type = GET_TICKS;
	send_recv(BOTH, TASK_SYS, &msg);
	return msg.RETVAL;
}


/**
 * @struct posix_tar_header
 * Borrowed from GNU `tar'
 */
struct posix_tar_header
{				/* byte offset */
	char name[100];		/*   0 */
	char mode[8];		/* 100 */
	char uid[8];		/* 108 */
	char gid[8];		/* 116 */
	char size[12];		/* 124 */
	char mtime[12];		/* 136 */
	char chksum[8];		/* 148 */
	char typeflag;		/* 156 */
	char linkname[100];	/* 157 */
	char magic[6];		/* 257 */
	char version[2];	/* 263 */
	char uname[32];		/* 265 */
	char gname[32];		/* 297 */
	char devmajor[8];	/* 329 */
	char devminor[8];	/* 337 */
	char prefix[155];	/* 345 */
	/* 500 */
};

/*****************************************************************************
 *                                untar
 *****************************************************************************/
/**
 * Extract the tar file and store them.
 * 
 * @param filename The tar file.
 *****************************************************************************/
void untar(const char * filename)
{
	printf("[extract `%s'\n", filename);
	int fd = open(filename, O_RDWR);
	assert(fd != -1);

	char buf[SECTOR_SIZE * 16];
	int chunk = sizeof(buf);
	int i = 0;
	int bytes = 0;

	while (1) {
		bytes = read(fd, buf, SECTOR_SIZE);
		assert(bytes == SECTOR_SIZE); /* size of a TAR file
					       * must be multiple of 512
					       */
		if (buf[0] == 0) {
			if (i == 0)
				printf("    need not unpack the file.\n");
			break;
		}
		i++;

		struct posix_tar_header * phdr = (struct posix_tar_header *)buf;

		/* calculate the file size */
		char * p = phdr->size;
		int f_len = 0;
		while (*p)
			f_len = (f_len * 8) + (*p++ - '0'); /* octal */

		int bytes_left = f_len;
		int fdout = open(phdr->name, O_CREAT | O_RDWR | O_TRUNC);
		if (fdout == -1) {
			printf("    failed to extract file: %s\n", phdr->name);
			printf(" aborted]\n");
			close(fd);
			return;
		}
		printf("    %s\n", phdr->name);
		while (bytes_left) {
			int iobytes = min(chunk, bytes_left);
			read(fd, buf,
			     ((iobytes - 1) / SECTOR_SIZE + 1) * SECTOR_SIZE);
			bytes = write(fdout, buf, iobytes);
			assert(bytes == iobytes);
			bytes_left -= iobytes;
		}
		close(fdout);
	}

	if (i) {
		lseek(fd, 0, SEEK_SET);
		buf[0] = 0;
		bytes = write(fd, buf, 1);
		assert(bytes == 1);
	}

	close(fd);

	printf(" done, %d files extracted]\n", i);
}

/*****************************************************************************
 *                                shabby_shell
 *****************************************************************************/
/**
 * A very very simple shell.
 * 
 * @param tty_name  TTY file name.
 *****************************************************************************/
PUBLIC void clear()
{
    int i = 0;
    for (i = 0; i < 30; i++)
        printf("\n");
}


void shabby_shell(const char * tty_name)
{
	int fd_stdin  = open(tty_name, O_RDWR);
	assert(fd_stdin  == 0);
	int fd_stdout = open(tty_name, O_RDWR);
	assert(fd_stdout == 1);

    char rdbuf[128]; //读取的命令
    char cmd[128];   //指令
    char arg1[128];  //参数1
    char arg2[128];  //参数2
    char buf[1024];

	initData();
	while (1)
	{
		login();
		while (1) 	
		{
			// write(1, "$ ", 2);
			printf("%s@MiniOS:%s$ ",currentUser,currentFolder);
			int r = read(0, rdbuf, 70);
			rdbuf[r] = 0;

			int argc = 0;
			char * argv[PROC_ORIGIN_STACK];
			char * p = rdbuf;
			char * s;
			int word = 0;
			char ch;
			do {
				ch = *p;
				if (*p != ' ' && *p != 0 && !word) {
					s = p;
					word = 1;
				}
				if ((*p == ' ' || *p == 0) && word) {
					word = 0;
					argv[argc++] = s;
					*p = 0;
				}
				p++;
			} while(ch);
			argv[argc] = 0;
			int fd = open(argv[0], O_RDWR);
			if (fd == -1) {
				if (rdbuf[0]) {
					// write(1, "{", 1);
					// write(1, rdbuf, r);
					// write(1, "}\n", 2);

					//执行命令
					if (strcmp(argv[0], "clear") == 0)
					{
						clear();
						welcome();
					} 
					else if (strcmp(argv[0], "users") == 0)
					{
						showUsers();
					}
					else if (strcmp(argv[0], "useradd") == 0)
					{
						if(argc==3)
						{
							addUser(argv[1], argv[2]);
						}
						else
						{
							printf("Usage:useradd [username] [password]\n");
						}
					}
					else if (strcmp(argv[0], "logout") == 0)
					{
						break;
					}
					else
					{
						printf("Command '%s' not found\n",argv[0]);
						continue;
					}
				}
			}
			else {
				close(fd);
				int pid = fork();
				if (pid != 0) { /* parent */
					int s;
					wait(&s);
				}
				else {	/* child */
					execv(argv[0], argv);
				}
			}
		}
		clear();
		welcome();
	}
	


	close(1);
	close(0);
}

/*****************************************************************************
 *                                Init
 *****************************************************************************/
/**
 * The hen.
 * 
 *****************************************************************************/
void Init()
{
	int fd_stdin  = open("/dev_tty0", O_RDWR);
	assert(fd_stdin  == 0);
	int fd_stdout = open("/dev_tty0", O_RDWR);
	assert(fd_stdout == 1);

	printf("Init() is running ...\n");

	/* extract `cmd.tar' */
	untar("/cmd.tar");
	welcomeAnimation();
    welcome();
			

	char * tty_list[] = {"/dev_tty0","/dev_tty1", "/dev_tty2"};

	int i;
	for (i = 0; i < sizeof(tty_list) / sizeof(tty_list[0]); i++) {
		int pid = fork();
		if (pid != 0) { /* parent process */
			// printf("[parent is running, child pid:%d]\n", pid);
		}
		else {	/* child process */
			// printf("[child is running, pid:%d]\n", getpid());
			close(fd_stdin);
			close(fd_stdout);
			
			shabby_shell(tty_list[i]);
			assert(0);
		}
	}

	while (1) {
		int s;
		int child = wait(&s);
		printf("child (%d) exited with status: %d.\n", child, s);
	}

	assert(0);
}
int vertify()
{
    if (currentUserIndex !=-1)
    {
        printf("Permission deny!!\n");
        return 0;
    }
    else
        return 1;
}

void initData()
{
    int fd = -1, n = 0, i = 0, count = 0, k = 0;
    char bufr[1024] = "";	//用户名数据缓冲区
    char bufp[1024] = "";	//密码数据缓冲区

	for(i=0;i<MAX_USER_COUNT;i++)
	{
		strcpy(users[i].username,"empty");
	}
	fd = open("Users", O_RDWR);
	if(fd==-1)
	{
		fd=open("Users",O_CREAT);
	}
	close(fd);
	fd = open("Passwords", O_RDWR);
	if(fd==-1)
	{
		fd=open("Passwords",O_CREAT);
	}
	close(fd);
	//读取用户名
    fd = open("Users", O_RDWR);
    n = read(fd, bufr, 1024);
    bufr[strlen(bufr)] = '\0';
    for (i = 0; i < strlen(bufr); i++)
    {
        if (bufr[i] != ' ')
        {
            users[count].username[k] = bufr[i];
            k++;
        }
        else
        {
            while (bufr[i] == ' ')
            {
                i++;
                if (bufr[i] == '\0')
                {
                    users[count].username[k] = '\0';
                    if (strcmp(users[count].username, "empty") != 0)
                        usercount++;
                    count++;
                    break;
                }
            }
            if (bufr[i] == '\0')
            {
                break;
            }
            i--;
            users[count].username[k] = '\0';
            if (strcmp(users[count].username, "empty") != 0)
                usercount++;
            k = 0;
            count++;
        }
    }
    close(fd);
    count = 0;
    k = 0;

	//读取用户密码
    fd = open("Passwords", O_RDWR);
    n = read(fd, bufp, 1024);
    for (i = 0; i < strlen(bufp); i++)
    {
        if (bufp[i] != ' ')
        {
			users[count].password[i] = bufp[i];
            k++;
        }
        else
        {
            while (bufp[i] == ' ')
            {
                i++;
                if (bufp[i] == '\0')
                {
                    count++;
                    break;
                }
            }
            if (bufp[i] == '\0')
                break;
            i--;
            users[count].password[k] = '\0';
            k = 0;
            count++;
        }
    }
    close(fd);
    count = 0;
    k = 0;

}
//登录
void login()
{
	while (1)
    {
        if (usercount == 0)
        {
            printf("Enter Admin Password:");
            char buf[128];
            int r = read(0, buf, 128);
            buf[r] = 0;
            if (strcmp(buf, "admin") == 0)
            {
                strcpy(currentUser, "Admin");
                break;
            }
            else
                printf("Password Error!\n");
        }
        else
        {
            //printf("%d",usercount);
            int isGet = 0;
            printf("User Name:");
            char buf[128];
            int r = read(0, buf, 128);
            buf[r] = 0;
            int i;
            for (i = 0; i < usercount; i++)
            {
                if (strcmp(buf, users[i].username) == 0 && strcmp(buf, "empty") != 0)
                {
                    printf("Password:");
                    char buf[128];
                    int r = read(0, buf, 128);
                    buf[r] = 0;
                    if (strcmp(buf,users[i].password) == 0)
                    {
                        strcpy(currentUser, users[i].username);
                        currentUserIndex=i;
                        isGet = 1;
                        break;
                    }
                }
            }
            if (isGet)
                break;
            else
                printf("Password Error Or User Not Exist!\n");
        }
    }
}

//显示所有用户
void showUsers()
{
	int i;
	printf("UserCount:%d\n",usercount);
	for (i = 0; i < usercount; i++)
	{
		if (strcmp(users[i].username, "empty") != 0)
		{
			printf("%s",users[i].username);
		}
	}
	printf("\n");
}

//添加用户
void addUser(char *username, char *password)
{
    if (currentUserIndex==-1)
    {
        int i;
		
        for (i = 0; i < usercount; i++)
        {
            if (strcmp(users[i].username, username) == 0)
            {
                printf("User exists!\n");
                return;
            }
        }
        if (usercount >= MAX_USER_COUNT)
        {
            printf("No more users\n");
            return;
        }
        if (strcmp(users[usercount].username, "empty") == 0)
        {
            strcpy(users[usercount].username, username);
            strcpy(users[usercount].password, password);
            usercount++;
            updateUsers();
			printf("Add User '%s' Successfully!\n",username);
            return;
        }
    }
    else
        printf("Permission Deny!\n");
}

//将用户数据保存到文件
void updateUsers()
{
    int i = 0, count = 0;
    editCover("Users", "");
	editCover("Passwords", "");
	for(i=0;i<usercount;i++)
	{
		if (strcmp(users[i].username, "empty") != 0)
		{
			editAppand("Users", users[i].username);
			editAppand("Users", " ");
			editAppand("Passwords",users[i].password);
        	editAppand("Passwords", " ");
		}
		else
		{
			editAppand("Users", "empty ");
			editAppand("Passwords", "empty ");
		}
	}
}

/* Edit File Cover */
void editCover(char *filepath, char *buf)
{

    if (vertify() == 0)
        return;

    int fd = -1;
    int n, i = 0;
    char bufr[1024] = "";
    char empty[1024];

    for (i = 0; i < 1024; i++)
        empty[i] = '\0';

    fd = open(filepath, O_RDWR);
    //printf("%d",fd);
    if (fd == -1)
        return;
    write(fd, empty, 1024);
    close(fd);
    fd = open(filepath, O_RDWR);
    write(fd, buf, strlen(buf));
    close(fd);
}

/* Edit File Appand */
void editAppand(char *filepath, char *buf)
{
    if (vertify() == 0)
        return;

    int fd = -1;
    int n, i = 0;
    char bufr[1024] = "";
    char empty[1024];

    for (i = 0; i < 1024; i++)
        empty[i] = '\0';
    fd = open(filepath, O_RDWR);
    if (fd == -1)
    {
        printf("Fail, please check and try again!!\n");
        return;
    }

    n = read(fd, bufr, 1024);
    n = strlen(bufr);

    for (i = 0; i < strlen(buf); i++, n++)
    {
        bufr[n] = buf[i];
        bufr[n + 1] = '\0';
    }
    write(fd, empty, 1024);
    fd = open(filepath, O_RDWR);
    write(fd, bufr, strlen(bufr));
    close(fd);
}

/*======================================================================*
                               TestA
 *======================================================================*/
void TestA()
{
	for(;;);
}

/*======================================================================*
                               TestB
 *======================================================================*/
void TestB()
{
	for(;;);
}

/*======================================================================*
                               TestB
 *======================================================================*/
void TestC()
{
	for(;;);
}

/*****************************************************************************
 *                                panic
 *****************************************************************************/
PUBLIC void panic(const char *fmt, ...)
{
	int i;
	char buf[256];

	/* 4 is the size of fmt in the stack */
	va_list arg = (va_list)((char*)&fmt + 4);

	i = vsprintf(buf, fmt, arg);

	printl("%c !!panic!! %s", MAG_CH_PANIC, buf);

	/* should never arrive here */
	__asm__ __volatile__("ud2");
}

