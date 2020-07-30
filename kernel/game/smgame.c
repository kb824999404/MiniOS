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


#define go(x,y) swap(&map[nx][ny],&map[nx+x][ny+y]),nx=nx+x,ny=ny+y

static const int centerX=SCREEN_WIDTH/2;
static const int centerY=PANELSIZE/2+TOPBAR_HEIGHT;
static int gridSizeX=5;
static int gridSizeY=5;

char map[100][100];
int maxscore;
int nx,ny;
int speed,score,downtime;
bool difficult=0;
bool tool=0;
bool died=0;
int nowt;
int st;
int _a,_b;
int o=0;//用于隔行生成'###'
PRIVATE bool ifdied(){
    if(nx==1||nx==20||map[nx+1][ny]=='^')return 1;
    return 0;
}
PRIVATE void swap(char* a,char* b)
{   
    char temp=*a;
    *a=*b;
    *b=temp;

}
PRIVATE void up(){
    nx-=1;
    if(ifdied())return;
    if(speed>200)speed-=5;
    if(speed<300)downtime=100;
    register int i,j;
    for(i=2;i<=19;i++){
        for(j=2;j<=49;j++){
        if(i!=19)map[i][j]=map[i+1][j]; 
        if(i==19)map[i][j]=' ';
        }
    }
    if(o==0)return;
    int fh=rand()%2==0?1:-1;
    int mid=ny+fh*rand()%6;
    int df=rand()%15;
    if(difficult==0)map[19][mid-1]=map[19][mid]=map[19][mid+1]='#';
    if(df==0)map[19][mid-1]=map[19][mid]=map[19][mid+1]='^';
    if(df==5)map[19][mid-1]=map[19][mid]=map[19][mid+1]='>';
    if(df==10)map[19][mid-1]=map[19][mid]=map[19][mid+1]='<';
    if(df==12){
        map[19][mid-1]=map[19][mid]=map[19][mid+1]='#';
    }
}//整体上移
PRIVATE void init(){
    st=time();
    speed=1000,score=0,downtime=20;
    register int i,j;
    for(i=1;i<=20;i++){
        for(j=1;j<=50;j++){
            map[i][j]=' ';
        }
    }
    //for(j=0;j<=50;j++)map[0][j]=map[21][j]='^';
    for(i=1;i<=20;i++)map[i][1]=map[i][50]='#';
    for(j=1;j<=50;j++)map[1][j]='#';
    int x,y;
    srand(time(0));
    x=15;
    y=25;
    map[x][y]='@';
    nx=x;
    ny=y;
    map[x+1][y-1]=map[x+1][y]=map[x+1][y+1]='#';
    return;
}
PRIVATE void down(){
    if(map[nx+1][ny]!='#'&&map[nx+1][ny]!='-'&&map[nx+1][ny]!='<'&&map[nx+1][ny]!='>'&&map[nx+1][ny]!='^')go(1,0);
}
int na,nb;
PRIVATE void now(){
    if(ifdied())return;
    nb=time();
    if(nb-na>=downtime)down(),na=nb;
    while(map[nx+1][ny]=='<')go(0,-1);
    while(map[nx+1][ny]=='>')go(0,1);
    if(map[nx+1][ny]=='-'){
	    if(_a!=0)_b=time();
	    if(_a==0)_a=time();
	    if(_b-_a>=1000)map[nx+1][ny]=' ',_a=_b=0;   
    }
    if(map[nx+1][ny]!='-')_a=_b=0;


    u32 c;
    c=kbhit();
        
    if(c=='a'||c=='A')
    {
        if(map[nx][ny-1]!='#'&&map[nx][ny-1]!='^'&&map[nx][ny-1]!='>'&&map[nx][ny-1]!='<'&&map[nx][ny-1]!='-')go(0,-1);//左 
    }
    else if(c=='d'||c=='D')
    {
        if(map[nx][ny+1]!='#'&&map[nx][ny+1]!='^'&&map[nx][ny+1]!='>'&&map[nx][ny+1]!='<'&&map[nx][ny+1]!='-')go(0,1);//右 
    }
    else if(c=='s'||c=='S')
    {
        if(map[nx+1][ny+1]==' ')go(1,0);//下
    }
    
        
}

PRIVATE void print(){
    if(ifdied())return;
    nowt=time();
    score=(nowt-st)/1000;
    register int i,j;
    unsigned c=COLOR_WHITE;
    for(i=1;i<=20;i++){
        for(j=1;j<=50;j++){

            switch(map[i][j])
            {
                case ' ':
                    c=COLOR_WHITE;
                    break;
                case '-':
                    c=COLOR_RED;
                    break;
                case '<':
                    c=COLOR_BRIGHTBLUE;
                    break;
                case '>':
                    c=COLOR_DARKGREEN;
                    break;
                case '#':
                    c=COLOR_BLACK;
                    break;
                case '@':
                    c=COLOR_BRIGHTPURPER;
                    break;
                default:
                    c=COLOR_WHITE;
                    break;
            }  
            drawBox(centerX-25*gridSizeX+j*gridSizeX,centerY-10*gridSizeY+i*gridSizeY,
                centerX-25*gridSizeX+(j+1)*gridSizeX,centerY-10*gridSizeY+(i+1)*gridSizeY,c);          
        }
    }
    return;
}
PRIVATE void game(){
    difficult=0;
    init(); 
    int a=time();
    int b;
    
    while(1){   
        if(ifdied())break;
        // milli_delay(10);
        now();  
        if(ifdied())break;
        b=time();
        if(b-a>=speed){
            o^=1;
            up();
            if(ifdied())break;  
            a=b;    
        }
        print();
    }
        


}

PRIVATE void initScreen()
{

    clearAll();
	drawFont(FONT_WIDTH,FONT_HEIGHT/2,"GAME: Move Game",COLOR_WHITE);
    drawFont(FONT_WIDTH+4*FONT_WIDTH,centerY,"Press Any Key to Start",COLOR_WHITE);
    drawFont(FONT_WIDTH,SCREEN_HEIGHT-FONT_HEIGHT*2,"[WD]:MOVE  [Q]:EXIT",COLOR_WHITE);
}
void runsmgame(){
	initScreen();
    game();
    printf("Final Score:%d\n",score);   
}
