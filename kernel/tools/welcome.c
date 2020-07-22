/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                            welcome.c
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
#include "graphics.h"



PUBLIC void welcome(){  
	int i=0;
	for(i=0;i<LINE_NUM/2-1;i++)
		printf("\n");
	for(i=0;i<(ROW_NUM-6)/2;i++)
		printf(" ");
	printf("MINIOS");
	for(i=0;i<(ROW_NUM-6)/2;i++)
		printf(" ");		
	for(i=0;i<LINE_NUM/2-1;i++)
		printf("\n");		
}

