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

static const int N=3;
static const int centerX=SCREEN_WIDTH/2;
static const int centerY=PANELSIZE/2+TOPBAR_HEIGHT;
static int gridSizeX=2*FONT_HEIGHT-2;
static int gridSizeY=2*FONT_HEIGHT-2;
int init_x,init_y;
char board[3][3];

PRIVATE void initScreen()
{
    clearAll();
	char s[40];
	sprintf(s,"GAME: Picrure Puzzle");
	drawFont(FONT_WIDTH,FONT_HEIGHT/2,s,COLOR_WHITE);
    drawFont(FONT_WIDTH,SCREEN_HEIGHT-FONT_HEIGHT*2,"[WASD]:MOVE  [Q]:EXIT",COLOR_WHITE);
}


PRIVATE bool judge(int x,int y)
{
    return x>=0&&x<N&&y>=0&&y<N;
}

PRIVATE void init_board()
{
    int i,j;
    for(i=0;i<N;i++)
    {
        for(j=0;j<N;j++)
        {
            if(i==N-1&&j==N-1)
                board[i][j]='*';
            else 
                board[i][j]=j+i*3+'0'+1;
        }
    }
    int x=N-1,y=N-1;
    int dx[4]={0,0,1,-1};
    int dy[4]={1,-1,0,0};
    unsigned int value=0;
    for(i=1;i<=15;i++)
    {
        value+=998244353;
        value*=19260817;
        int p=value%4;
        int nx=x+dx[p];
        int ny=y+dy[p];
        if(judge(nx,ny))
        {
            char tmp=board[x][y];
            board[x][y]=board[nx][ny];
            board[nx][ny]=tmp;

            x=nx;y=ny;
        }
    }
    init_x=x;init_y=y;
}

PRIVATE bool sucess()
{
    int i,j;
    for(i=0;i<N;i++)
    {
        for(j=0;j<N;j++)
        {
            if(i==N-1&&j==N-1)continue;
            if(board[i][j]=='*')return false;
        }
    }
    return true;
}

PRIVATE void print_board()
{
    drawBox(centerX-3*gridSizeX/2,centerY-3*gridSizeY/2,centerX+3*gridSizeX/2,centerY+3*gridSizeY/2,COLOR_WHITE);
	int i, j;
	for(i=0;i<=N;i++)
	{
        drawLine(centerX-3*gridSizeX/2+i*gridSizeX,centerY-3*gridSizeY/2,centerX-3*gridSizeX/2+i*gridSizeX,centerY+3*gridSizeY/2,COLOR_BLACK);
        drawLine(centerX-3*gridSizeX/2,centerY-3*gridSizeY/2+i*gridSizeY,centerX+3*gridSizeX/2,centerY-3*gridSizeY/2+i*gridSizeY,COLOR_BLACK);
	}
    for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
            char s[20];
            sprintf(s,"%c",board[i][j]);
            int x0=centerX-3*gridSizeX/2+j*gridSizeX+gridSizeX/2-FONT_WIDTH/2;
            int y0=centerY-3*gridSizeY/2+i*gridSizeY+gridSizeY/2-FONT_HEIGHT/2;
            drawFont(x0,y0,s,COLOR_BLACK);
		}
	}
}

PRIVATE void work()
{
    init_board();
    print_board();
    int x=init_x,y=init_y;
    while(1)
    {
        u32 key=kbhit();
        
        int nx,ny;
        switch (key)
        {
            case 'a':
            case 'A':
                nx=x;ny=y-1;
                break;
            case 's':
            case 'S':
                nx=x+1;ny=y;
                break;
            case 'w':
            case 'W':
                nx=x-1;ny=y;
                break;
            case 'd':
            case 'D':
                nx=x;ny=y+1;
                break;
            case 'q':
            case 'Q':
                clearAll();
                return;
        }       
        if(!judge(nx,ny))
        {
            continue;
        }
        char tmp=board[x][y];
        board[x][y]=board[nx][ny];
        board[nx][ny]=tmp;
        print_board(board);
        x=nx,y=ny;

        if(sucess(board)){
            clearAll();
			char *s="You Win!";
			drawFont(centerX-strlen(s)/2*FONT_WIDTH,centerY,s,COLOR_RED);
            break;
        }
    }
}

void runpingtu()
{
	

    while(1)
    {
        initScreen();
        work();
        char *s="TRY THE GAME AGAIN? [Y/N]";
        drawFont(centerX-strlen(s)/2*FONT_WIDTH,centerY+FONT_HEIGHT,s,COLOR_BRIGHTYELLOW);

        u32 key=kbhit();
        if(!(key=='y'||key=='Y'))
            return;

    }

}
