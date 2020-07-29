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
#include "keyboard.h"

// 推箱子
int pi=0;
int pj=0;
int count;
static const int centerX=SCREEN_WIDTH/2;
static const int centerY=PANELSIZE/2+TOPBAR_HEIGHT;
static int gridSizeX=10;
static int gridSizeY=10;

void draw_map(int map[9][11])
{
    int i,j;
    unsigned c=COLOR_WHITE;
    // drawBox(centerX-5*gridSizeX,centerY-5*gridSizeY,centerX+6*gridSizeX,centerY+5*gridSizeY,COLOR_WHITE);
    for(i=0; i<9; i++)
    {
        for(j=0; j<11; j++)
        {

            switch(map[i][j])
            {
                case 0:             // 道路
                c=COLOR_WHITE;
                break;
                case 1:             // 墙壁
                // printf("#");        
                c=COLOR_BLACK;
                break;
                case 2:          // 游戏边框空白部分
                c=COLOR_WHITE;
                // printf(" ");        
                break;
                case 3:             // 目的地
                // printf("D");        
                c=COLOR_BRIGHTYELLOW;
                break;      
                case 4:             // 箱子
                // printf("b");        
                c=COLOR_BRIGHTBLUE;
                break;
                case 7:             // 箱子进入目的地
                // printf("!");        
                c=COLOR_BRIGHTGREEN;
                break;
                case 6:             // 人
                // printf("p");        
                c=COLOR_RED;
                break;  
                case 9:             // 人进入目的地
                // printf("^");        
                c=COLOR_DARKPURPER;
                break;
            }  
            drawBox(centerX-11/2*gridSizeX+j*gridSizeX,centerY-9/2*gridSizeY+i*gridSizeY,
                centerX-11/2*gridSizeX+(j+1)*gridSizeX,centerY-9/2*gridSizeY+(i+1)*gridSizeY,c);          
        }
    }
}

void boxMenu()
{
    clearAll();
	char s[40];

    sprintf(s,"GAME: Push Box Score:%d",count);
	drawFont(FONT_WIDTH,FONT_HEIGHT/2,s,COLOR_WHITE);
    drawFont(FONT_WIDTH,SCREEN_HEIGHT-FONT_HEIGHT*2,"[WASD]:MOVE [Q]:EXIT",COLOR_WHITE);
}

void runpushbox()
{
    u32 control;

    while (1)  
    {
        // 地图定义
        int map[9][11] = {
            {2,1,1,1,1,1,1,1,1,1,2},
            {2,1,0,0,0,1,0,0,0,1,2},
            {2,1,0,4,4,4,4,4,0,1,2},
            {2,1,0,4,0,4,0,4,0,0,1},
            {2,1,0,0,0,6,0,0,4,0,1},
            {1,1,0,1,1,1,1,0,4,0,1},
            {1,0,3,3,3,3,3,1,0,0,1},
            {1,0,3,3,3,3,3,0,0,0,1},
            {1,1,1,1,1,1,1,1,1,1,2},
        };
        count = 0;   // 记分变量
		boxMenu();
        draw_map(map);
        // while(1);

        // 找人初始位置
        for (pi = 0; pi < 9; pi++)
        {
            for (pj = 0; pj < 11; pj++)
            {
                if (map[pi][pj]==6||map[pi][pj]==9)
                break;
            }
            if (map[pi][pj]==6||map[pi][pj]==9)
            break;
        }
        while (1)
        {
            control=kbhit();
            
            if(control == 'Q' || control == 'q')
            {
                clearAll();
                break;
            }
            switch (control)
            {
                case 'w':
                case 'W':
                // 若人前面是空地。  
                if(map[pi-1][pj] == 0)
                {
                    map[pi-1][pj] = 6+0; 
                    if(map[pi][pj] == 9) 
                        map[pi][pj] = 3; 
                    else
                        map[pi][pj] = 0;    
                    pi--;
                }
                // 若人前面是目的地。
                else if(map[pi-1][pj] == 3)
                {
                    map[pi-1][pj] = 6+3; 
                    if(map[pi][pj] == 9) 
                        map[pi][pj] = 3; 
                    else
                        map[pi][pj] = 0; 
                    pi--;
                }
                // 若人前面是箱子。
                else if(map[pi-1][pj] == 4)
                {
                    if (map[pi-2][pj] == 0)
                    {
                        map[pi-2][pj] = 4; 
                        if(map[pi-1][pj] == 7) 
                            map[pi-1][pj] = 9; 
                        else
                            map[pi-1][pj] = 6; 
                        if(map[pi][pj] == 9) 
                            map[pi][pj] = 3; 
                        else
                            map[pi][pj] = 0; 
                        pi--;
                    }
                    // 若人的前面是箱子，而箱子前面是目的地。
                    else if (map[pi-2][pj] == 3)
                    {
                        map[pi-2][pj] = 7;
                        count++;
                        if(map[pi-1][pj] == 7)
                            map[pi-1][pj] = 9;
                        else
                            map[pi-1][pj] = 6;
                        if(map[pi][pj] == 9)
                            map[pi][pj] = 3;
                        else
                            map[pi][pj] = 0;
                        pi--;
                    }
                }
                // 若人前面是已经进入某目的地的箱子
                else if(map[pi-1][pj] == 7)
                {
                    // 若人前面是已经进入某目的地的箱子,箱子前面是空地。
                    if(map[pi-2][pj] == 0)
                    {
                        count--;
                        map[pi-2][pj] = 4;   
                        map[pi-1][pj] = 9;   
                        if(map[pi][pj] == 9)
                            map[pi][pj] = 3;
                        else
                            map[pi][pj] = 0;
                        pi--;
                    }
                    // 若人前面是已经进入某目的地的箱子，箱子前面是另一目的地。
                    if(map[pi-2][pj] == 3)
                    {
                        map[pi-2][pj] = 7;   
                        map[pi-1][pj] = 9;   
                        if(map[pi][pj] == 9)
                            map[pi][pj] = 3;
                        else
                            map[pi][pj] = 0;
                        pi--;
                    }
                }
                break;
                case 's':
                case 'S':
                // 若人前面是空地。
                if(map[pi+1][pj] == 0)
                {
                    map[pi+1][pj] = 6+0;
                    if(map[pi][pj] == 9) 
                        map[pi][pj] = 3; 
                    else
                        map[pi][pj] = 0;   
                    pi++; 
                }
                // 若人前面是目的地。
                else if(map[pi+1][pj] == 3)
                {
                    map[pi+1][pj] = 6+3; 
                    if(map[pi][pj] == 9) 
                        map[pi][pj] = 3;
                    else
                        map[pi][pj] = 0; 
                    pi++;
                }
                // 若人前面是箱子。
                else if(map[pi+1][pj] == 4)
                {
                    // 若人前面是箱子，而箱子前面是空地。
                    if (map[pi+2][pj] == 0)
                    {
                        map[pi+2][pj] = 4; 
                        if(map[pi+1][pj] == 7) 
                            map[pi+1][pj] = 9; 
                        else
                            map[pi+1][pj] = 6;
                        if(map[pi][pj] == 9) 
                            map[pi][pj] = 3; 
                        else
                            map[pi][pj] = 0; 
                        pi++;
                    }
                    // 若人的前面是箱子，而箱子前面是目的地。
                    else if (map[pi+2][pj] == 3)
                    {
                        map[pi+2][pj] = 7;
                        count++;
                        if(map[pi+1][pj] == 7)
                            map[pi+1][pj] = 9;
                        else
                            map[pi+1][pj] = 6;
                        if(map[pi][pj] == 9)
                            map[pi][pj] = 3;
                        else
                            map[pi][pj] = 0;
                        pi++;
                    }
                }
                else if(map[pi+1][pj] == 7)
                {
                    if(map[pi+2][pj] == 0)
                    {
                        count--;
                        map[pi+2][pj] = 4;  
                        map[pi+1][pj] = 9;  
                        if(map[pi][pj] == 9)
                            map[pi][pj] = 3;
                        else
                            map[pi][pj] = 0;
                        pi++;
                    }
                    if(map[pi+2][pj] == 3)
                    {
                        map[pi+2][pj] = 7; 
                        map[pi+1][pj] = 9;   
                        if(map[pi][pj] == 9)
                            map[pi][pj] = 3;
                        else
                            map[pi][pj] = 0;
                        pi++;
                    }
                }
                break;
                case 'a':
                case 'A':
                if(map[pi][pj-1] == 0)
                {
                    map[pi][pj-1] = 6+0; 
                    if(map[pi][pj] == 9) 
                        map[pi][pj] = 3; 
                    else
                        map[pi][pj] = 0;    
                    pj--;
                }
                else if(map[pi][pj-1] == 3)
                {
                    map[pi][pj-1] = 6+3;
                    if(map[pi][pj] == 9) 
                        map[pi][pj] = 3;
                    else
                        map[pi][pj] = 0; 
                    pj--;
                }
                else if(map[pi][pj-1] == 4)
                {
                    if (map[pi][pj-2] == 0)
                    {
                        map[pi][pj-2] = 4; 
                        if(map[pi][pj-1] == 7) 
                            map[pi][pj-1] = 9; 
                        else
                            map[pi][pj-1] = 6; 
                        if(map[pi][pj] == 9) 
                            map[pi][pj] = 3; 
                        else
                            map[pi][pj] = 0; 
                        pj--;
                    }
                    else if (map[pi][pj-2] == 3)
                    {
                        count++;
                        map[pi][pj-2] = 7;  
                        if(map[pi][pj-1] == 7)
                            map[pi][pj-1] = 9;
                        else
                            map[pi][pj-1] = 6;
                        if(map[pi][pj] == 9)
                            map[pi][pj] = 3;
                        else
                            map[pi][pj] = 0;
                        pj--;
                    }
                }
                else if(map[pi][pj-1] == 7)
                {
                    if(map[pi][pj-2] == 0)
                    {
                        count--;
                        map[pi][pj-2] = 4;   
                        map[pi][pj-1] = 9;
                        if(map[pi][pj] == 9) 
                            map[pi][pj] = 3;
                        else
                            map[pi][pj] = 0;
                        pj--;
                    }
                    if(map[pi][pj-2] == 3)
                    {
                        map[pi][pj-2] = 7;   
                        map[pi][pj-1] = 9;
                        if(map[pi][pj] == 9) 
                            map[pi][pj] = 3;
                        else
                            map[pi][pj] = 0;
                        pj--;
                    }
                }
                break;
                case 'd':
                case 'D':
                if(map[pi][pj+1] == 0)
                {
                    map[pi][pj+1] = 6+0; 
                    if(map[pi][pj] == 9) 
                        map[pi][pj] = 3; 
                    else
                        map[pi][pj] = 0;   
                    pj++;       
                }
                else if(map[pi][pj+1] == 3)
                {
                    map[pi][pj+1] = 6+3; 
                    if(map[pi][pj] == 9) 
                        map[pi][pj] = 3; 
                    else
                        map[pi][pj] = 0; 
                    pj++;
                }
                else if(map[pi][pj+1] == 4)
                {
                    if (map[pi][pj+2] == 0)
                    { 
                        map[pi][pj+2] = 4; 
                        if(map[pi][pj+1] == 7) 
                            map[pi][pj+1] = 9; 
                        else
                            map[pi][pj+1] = 6; 
                        if(map[pi][pj] == 9) 
                            map[pi][pj] = 3; 
                        else
                            map[pi][pj] = 0; 
                        pj++;
                    }
                    else if (map[pi][pj+2] == 3)
                    { 
                        count++;
                        map[pi][pj+2] = 7;   
                        if(map[pi][pj+1] == 7)
                            map[pi][pj+1] = 9;
                        else
                            map[pi][pj+1] = 6;
                        if(map[pi][pj] == 9)
                            map[pi][pj] = 3;
                        else
                            map[pi][pj] = 0;
                        pj++;
                    }
                }
                else if(map[pi][pj+1] == 7)
                {
                    if(map[pi][pj+2] == 0)
                    {
                        count--;
                        map[pi][pj+2] = 4;   
                        map[pi][pj+1] = 9;  
                        if(map[pi][pj] == 9) 
                            map[pi][pj] = 3;
                        else
                            map[pi][pj] = 0;
                        pj++;
                    }
                    if(map[pi][pj+2] == 3)
                    {
                        map[pi][pj+2] = 7;  
                        map[pi][pj+1] = 9;   
                        if(map[pi][pj] == 9) 
                            map[pi][pj] = 3;
                        else
                            map[pi][pj] = 0;
                        pj++;
                    }
                }
                break;
            }
            if(count == 8)   
            {
                clearAll();
                char *s="You Win!";
                drawFont(centerX-strlen(s)/2*FONT_WIDTH,centerY,s,COLOR_RED);
                break;
            }
            boxMenu();
            draw_map(map);
        }
        char *s="TRY THE GAME AGAIN? [Y/N]";
        drawFont(centerX-strlen(s)/2*FONT_WIDTH,centerY+FONT_HEIGHT,s,COLOR_BRIGHTYELLOW);

        u32 key=kbhit();
        if(!(key=='y'||key=='Y'))
            break;
        

    }
}
