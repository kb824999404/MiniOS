
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                            user.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                                                    Kaibin Zhou, 2020
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
#include "tools/user.h"

//用户数据
struct Users users[MAX_USER_COUNT];
char currentUser[128] = "/";
int usercount = 0;
int currentUserIndex=-1;


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

void initUsers()
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

//根据用户名查找用户下标
int searchUser(char *username)
{
    int i;
    for (i = 0; i < usercount; i++)
    {
        if (strcmp(users[i].username, username) == 0&& strcmp(username, "empty") != 0)
        {
            return i;
        }
    }
    return -1;
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
                currentUserIndex=-1;
                strcpy(currentUser, "Admin");
                break;
            }
            else
                printf("Password Error!\n");
        }
        else
        {
            printf("User Name:");
            char buf[128];
            int r = read(0, buf, 128);
            buf[r] = 0;
            int i;
            i=searchUser(buf);
            if(i!=-1)       //找到用户
            {
                printf("Password:");
                char buf[128];
                int r = read(0, buf, 128);
                buf[r] = 0;
                if (strcmp(buf,users[i].password) == 0)
                {
                    strcpy(currentUser, users[i].username);
                    currentUserIndex=i;
                    break;
                }
                printf("Password Error Or User Not Exist!\n");
            }
            else        //找不到用户，判断是否为管理员
            {
                if (strcmp(buf, "Admin") == 0)
                {
                    printf("Password:");
                    char buf[128];
                    int r = read(0, buf, 128);
                    buf[r] = 0;
                    if (strcmp(buf,"admin") == 0)
                    {
                        strcpy(currentUser, "Admin");
                        currentUserIndex=-1;
                        break;
                    }
                }
                printf("Password Error Or User Not Exist!\n");
            }
        }
    }
    topBar(currentUser);
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
        if (strcmp("empty", username) == 0)
        {
            printf("Invalid Username!\n");
            return;
        }	
        if (strcmp("Admin", username) == 0)
        {
            printf("User exists!\n");
            return;
        }		
        if(searchUser(username)!=-1)
        {
            printf("User exists!\n");
            return;
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

//删除用户
void delUser(char *username)
{
    if (currentUserIndex== -1)
    {
        int i = 0,j=0;
        i=searchUser(username);
        if(i!=-1)
        {
            printf("Confirm to del user %s?(Y/N)",username);
            int r;
            char buf[128];
            r = read(0, buf, 128);
            buf[r] = 0;
            if(!(strcmp(buf,"Y")==0||strcmp(buf,"y")==0))
                return;

            usercount--;
            for(j=i;j<usercount;j++)
            {
                strcpy(users[j].username,users[j+1].username);
                strcpy(users[j].password,users[j+1].password);
            }
            updateUsers();
            printf("Del User %s Successfully!\n",username);
            return;
        }
        else
            printf("Sorry! No such user!\n");
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
void passwd(char *username)
{
    if(currentUserIndex==-1)        //管理员能修改所有用户密码
    {
        if(strcmp(username,"Admin")==0)
        {
            printf("Admin Password No Modify!\n");
            return;
        }
        int i=searchUser(username);
        if(i!=-1)
        {
            modifyPassword(i);
        }
        else
            printf("Sorry! No such user!\n");
    }
    else                            //普通用户只能修改自己的密码
    {
        if(strcmp(username,currentUser)==0)
        {
            modifyPassword(currentUserIndex);
        }
        else
            printf("Permission Deny!\n");
    }
    
}
//修改用户密码
void modifyPassword(int index)
{
    char bufo[128];
    char bufn1[128];
    char bufn2[128];
    int i,r;
    if(currentUserIndex!=-1)
    {
        printf("Old Password:");
        r = read(0, bufo, 128);
        bufo[r] = 0;
    }
    if(currentUserIndex==-1||strcmp(bufo,users[index].password) == 0)
    {
        printf("New Password:");
        r = read(0, bufn1, 128);
        bufn1[r] = 0;
        printf("Confirm New Password:");
        r = read(0, bufn2, 128);
        bufn2[r] = 0;
        if(strcmp(bufn1,bufn2)==0)
        {
            strcpy(users[index].password, bufn1);
            int temp=index;      
            currentUserIndex=-1;            //获取管理员权限
            updateUsers();
            currentUserIndex=temp;
            printf("Modify %s Password Successfully!\n",users[index].username);
        }
        else
            printf("Two different input!\n");
    }
    else
        printf("Password Error!\n");
}
//获取管理员权限
void sudo()
{
    if(currentUserIndex!=-1)
    {
        printf("Enter Admin Password:");
        char buf[128];
        int r = read(0, buf, 128);
        buf[r] = 0;
        if (strcmp(buf, "admin") == 0)
        {
            currentUserIndex=-1;
            strcpy(currentUser, "Admin");
        }
        else
            printf("Password Error!\n");        
    }
}

