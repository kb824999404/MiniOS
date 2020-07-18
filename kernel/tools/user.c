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