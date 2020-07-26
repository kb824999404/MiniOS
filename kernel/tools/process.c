
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


//显示进程
void showProcess()
{	int i = 0;
	printf("---------------------------------\n");
    printf("| pid |    name     |   state   |\n");
    printf("---------------------------------\n");
	for (i = 0; i < NR_TASKS + NR_NATIVE_PROCS; i++)
	{
		if(proc_table[i].p_flags != 1){
			printf("|  %d",proc_table[i].pid);
			if(proc_table[i].pid<10) printf("  ");
			else if(proc_table[i].pid<100) printf(" ");

        		printf("| %s",proc_table[i].name);
			int j;
        		for(j=0;j<12-strlen(proc_table[i].name);j++)
            			printf(" ");

			if(proc_table[i].run_state) printf("|  running ");
			else printf("|  paused  ");

			if(i <= NR_TASKS) printf("-");
			else printf(" ");
        		printf("|\n");	
		}
	}
	printf("---------------------------------\n");
}


//停止进程
void killpro(char *a)
{
    int b = *a - 48;
    if(b >= 0 && b <= NR_TASKS)   printf("operation to proc %d denied \n",b);
    else if(b < NR_TASKS + NR_NATIVE_PROCS)
    {
        proc_table[b].p_flags = 1;
        showProcess();
    }
    else printf("can not find proc with pid:%d \n",b);
}

//暂停进程
void pausepro(char *a)
{
    int b = *a - 48;
    if(b >= 0 && b <= NR_TASKS)   printf("operation to proc %d denied \n",b);
    else if(b < NR_TASKS + NR_NATIVE_PROCS)
    {
        proc_table[b].run_state = 0;
        showProcess();
    }
    else printf("can not find proc with pid:%d \n",b);
}

//恢复进程
void resume(char *a)
{
    int b = *a - 48;
    if(b >= 0 && b <= NR_TASKS)   printf("operation to proc %d denied \n",b);
    else if(b < NR_TASKS + NR_NATIVE_PROCS)
    {
        proc_table[b].run_state = 1;
        showProcess();
    }
    else printf("can not find proc with pid:%d \n",b);
}