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


void welcomeAnimation(){
	int t=200;
	milli_delay(t);
	int i = 0;
	for (i = 1; i <= 100; ++i)
	{
		printf("-");
	}
	for (i = 1; i <= 100; ++i)
	{
		printf("\b");
	}
	for (i = 1; i <= 160; ++i)
	{
		printf(">");
		milli_delay(t);
	}

	printf("\n");
	milli_delay(t);
}

PUBLIC void welcome(){   
	printf("================================================================================\n");
	printf("\n");
	printf("             ####    #######          ###    ######     #######              \n"); 
 	printf("             ####   #####                   ###  ####  ###  ####             \n");
 	printf("             ####   #####                  ###    #######    ###             \n");
 	printf("             ##### ######### #### ### ### ###      ### ###                   \n");
 	printf("             ##### ######### ###   ###### ###      ###  ######               \n");
 	printf("             ######## ###### ###   ###### ###      ###     #####             \n");
 	printf("             ######## ###### ###   ###### ###      ###       ###             \n");
 	printf("             ### #### ###### ###   ###### ###      ###        ###            \n");
 	printf("             ### ###  ###### ###   ######  ###    #######    ###             \n");
 	printf("             ###      ###### ###   ######   ###  ####  ###   ###             \n");
 	printf("             ###      ###### ###   ######    ######     #######              \n");                                       
	printf(" |=================================MiniOS====================================|\n");
	printf(" |                             1851197 Kaibin Zhou                           |\n");
	printf(" |                             1851352 Rui Fu                                |\n");
	printf(" |                             1851199 Zeyang Zhuang                         |\n");
	printf(" |                             1851494 Yuqi Wang                             |\n");
	printf(" |                             1851198 Jiayao Lv                             |\n");
	printf(" |===========================================================================|\n");
	printf("\n");
	printf("================================================================================\n");
	//printf("help\n");
}
