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

void runguess()
{
    int n=rand()%1000,g,count=0;;
    while (true)
    {
        if(count>=10)
        {
            printf("Loss!You have guess %d times!\n",count);
            return;
        }
        printf("Please Guess The Number[0~1000]:");
        g=readint();
        if(g==n)
        {
            printf("Right!The number is %d\n",n);
            return;
        }
        else if (g<n)
        {
            printf("Wrong!Too Small!\n");
        }
        else
        {
            printf("Wrong!Too Big!\n");
        }
        count++;
    }
}