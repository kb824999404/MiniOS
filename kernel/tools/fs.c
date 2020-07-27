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
#include "tools/fs.h"
#include "tools/user.h"

//文件数据
char currentFolder[128] = "/";
char filepath[128] = "";
char files[20][128];
int fileCount=0;


//覆盖文件
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
    if (fd == -1)
    {
        fd = open(filepath, O_CREAT);
    }
    write(fd, empty, 1024);
    close(fd);
    fd = open(filepath, O_RDWR);
    write(fd, buf, strlen(buf));
    close(fd);
}

//追加文件
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

/* Ls */
void ls()
{
    readUserFiles();
    int i;
    for(i=0;i<fileCount;i++)
    {
        printf("%s ",files[i]);
    }
    printf("\n");
}


void pathFilter(char* bufr)
{
	char *p,*q;
	char temp[128];
	int length = 0;
	p = bufr;
	q = p;
    fileCount=0;
	while(1)
	{
		while(*q != ' ' && *q != '\0')
		{
			q++;												
		}
		if(*q == '\0')
			return;
		else
		{
			*q = '\0';
			length = q - p;
			int i = 0;
			for(i;i < length; i++,p++)
			{
				temp[i] = *p;
			}
			temp[i] = '\0';
            strcpy(files[fileCount++],temp);
			q++;
			p = q;
		}
	}
}

//创建文件路径
void createFilepath(char* filename)
{
	int i = 0, j = 0, k = 0;
		
	for (i; i < strlen(currentUser); i++)
	{
		filepath[i] = currentUser[i];
	}
    // filepath[i++]='/';

	for(k = 0; k < strlen(filename); i++, k++)
	{	
		filepath[i] = filename[k];
	}
	filepath[i] = '\0';
}


/* 创建文件*/
void createFile(char *file)
{
    createFilepath(file);
    int fd = -1, i = 0;
    fd = open(filepath,O_RDWR);
    if (fd == -1)   //文件不存在
    {
        fd=open(filepath,O_CREAT);  //新建文件
        close(fd);
        printf("Content:");
        char buf[1024];
        int r = read(0, buf, 1024);
        buf[r] = 0;
        editCover(filepath,buf);
        strcpy(files[fileCount++],file);
        updateUserFiles();
    }
    else    //文件已存在
    {
        close(fd);
        printf("Fail, file exsists!!\n");
    }

    clearArr(filepath, 128);

}

/* 读取文件*/
void readFile(char *file)
{
    createFilepath(file);
    int fd = -1, i = 0,n;
    char bufr[1024];
    fd = open(filepath,O_RDWR);
    if (fd != -1)   //文件存在
    {
        n = read(fd, bufr, 1024);
        bufr[n] = '\0';
        printf("%s",bufr);
        printf("\n");
        close(fd);
    }
    else    //文件不存在
    {
        printf("Fail, file not exsists!!\n");
    }

    clearArr(filepath, 128);

}

//删除文件
void delFile(char *file)
{
    createFilepath(file);
    int fd = -1, i = 0,n;
    char bufr[1024];
    fd = open(filepath,O_RDWR);
    if (fd != -1)   //文件存在
    {
        close(fd);
        editCover(filepath, "");
        int a = unlink(filepath);
        if (a != 0)
        {
            printf("Edit fail, please try again!\n");
            return;
        }
        readUserFiles();
        for(i=0;i<fileCount;i++)
        {
            if(strcmp(files[i],file)==0)
                break;
        }
        fileCount--;
        for(;i<fileCount;i++)
        {
            strcpy(files[i],files[i+1]);
        }
        updateUserFiles();
        printf("Delete file %s successfully!\n",file);
    }
    else    //文件不存在
    {
        printf("Fail, file not exsists!!\n");
    }

    clearArr(filepath, 128);

}
//覆盖文件
void ModifyFile(char *file)
{
    createFilepath(file);
    int fd = -1, i = 0,n;
    char bufr[1024];
    fd = open(filepath,O_RDWR);
    if (fd != -1)   //文件存在
    {
        printf("Content:");
        char buf[1024];
        int r = read(0, buf, 1024);
        buf[r] = 0;
        editCover(filepath,buf);
    }
    else    //文件不存在
    {
        printf("Fail, file not exsists!!\n");
    }

    clearArr(filepath, 128);
}
//追加文件
void AppendFile(char *file)
{
    createFilepath(file);
    int fd = -1, i = 0,n;
    char bufr[1024];
    fd = open(filepath,O_RDWR);
    if (fd != -1)   //文件存在
    {
        printf("Content:");
        char buf[1024];
        int r = read(0, buf, 1024);
        buf[r] = 0;
        editAppand(filepath,buf);
    }
    else    //文件不存在
    {
        printf("Fail, file not exsists!!\n");
    }

    clearArr(filepath, 128);
}

//清空数组
void clearArr(char *arr, int length)
{
    int i;
    for (i = 0; i < length; i++)
        arr[i] = 0;
}
// 读取用户文件列表
void readUserFiles()
{
	int fd = -1, n,i;
	char bufr[1024];
    fd = open(currentUser, O_RDWR);
    if (fd == -1)
    {
        fd=open(currentUser,O_CREAT);
        printf("\n");
        return;
    }
    n = read(fd, bufr, 1024);
    pathFilter(bufr);												
    close(fd);
}
// 更新用户文件列表
void updateUserFiles()
{
    int i = 0, count = 0;
    editCover(currentUser,"");
	for(i=0;i<fileCount;i++)
	{
        editAppand(currentUser,files[i]);
        editAppand(currentUser, " ");
	}
}

