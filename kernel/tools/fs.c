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
char userfiles[20][128];
int filequeue[50];
int filecount = 0;
int isEntered = 0;
int leiflag = 0;

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
        return;
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
