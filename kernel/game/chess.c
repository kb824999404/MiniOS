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

#undef X1
#undef X2
#undef Y1
#undef Y2


// 初始变量预定义
# define INT_MAX   999999
# define INT_MIN   (-INT_MAX - 1)
# define SPA 0
# define MAN 1
# define COM 2
# define NULL ((void*)0)    /* 空位置设为0，玩家棋设为1，电脑棋设为2 */


int chess[10][10];          /* 10*10的棋盘 */
int a, b;                   /* a b为玩家下棋坐标*/
int c, d, x;                /* c d为电脑下棋坐标，x为剩余空位置*/
int currentX=0,currentY=0;

int isStart = 0;
int isPainting = 0;
int state = 1;              /* 1为无效，0为有效*/
static const int centerX=SCREEN_WIDTH/2;
static const int centerY=PANELSIZE/2+TOPBAR_HEIGHT;
static int gridSizeX=10;
static int gridSizeY=10;
static int chessSize=4;

// 函数声明
PRIVATE void start();    /* 程序开始 */
PRIVATE void draw();                                /* 棋盘绘制 */
PRIVATE int win(int p,int q);                       /* 由棋子判断胜利情况 */
PRIVATE void AI(int *p,int *q);                     /* 电脑下棋返回p q棋子坐标 */
PRIVATE int value(int p,int q);                     /* 估值函数 */

PRIVATE int formationStrategy(int n, int p, int q); /* 棋型号判断 */
PRIVATE void moveStep(int n, int *x, int *y);       /* 棋盘棋子移动 */


PRIVATE void start()
{
    int a1 = 0, b1 = 0, c1 = 0, d1 = 0;     // 棋子坐标
    char i, p,j;
    char buf[100]={0};

    clearAll();
	char s[40];

    sprintf(s,"GAME: Gobang Chess");
	drawFont(FONT_WIDTH,FONT_HEIGHT/2,s,COLOR_WHITE);
    drawFont(FONT_WIDTH,SCREEN_HEIGHT-FONT_HEIGHT*2,"[WASD]:MOVE [SPACE]:Confirm [Q]:EXIT",COLOR_WHITE);

    for(j = 0; j < 10; j++)
        for( i = 0; i < 10; i++)
            chess[j][i] = SPA;              // 棋盘初始化
    draw();

    while(x != 0){
        currentX=currentY=0;
        if(x == 225) {
            c = 7;
            d = 7;
            chess[d][c]=COM;
            x--;
            draw();
        }       /* 电脑初始下棋 */
        else {
            AI(&c,&d);
            chess[d][c]=COM;
            x--;
            draw();
        }       /* 电脑AI下棋 */

        if(win(c,d)){       /* 若电脑赢 */
            clearAll();
			char *s="Computer Win!";
			drawFont(centerX-strlen(s)/2*FONT_WIDTH,centerY,s,COLOR_RED);
            return;
        }
        while (1)
        {
            u32 key=kbhit();
            bool confirm=false;
            switch (key)
            {
                case 'a':
                case 'A':
                    if(currentX>0)
                        currentX--;
                    break;
                case 's':
                case 'S':
                    if(currentY<9)
                        currentY++;
                    break;
                case 'w':
                case 'W':
                    if(currentY>0)
                        currentY--;
                    break;
                case 'd':
                case 'D':
                    if(currentX<9)
                        currentX++;
                    break;
                case 'q':
                case 'Q':
                    clearAll();
                    return;
                case ' ':
                    confirm=true;

            }  
            if(confirm)
            {
                if(chess[currentY][currentX]==SPA)
                    break;
            } 
            draw();    
        }
        b=currentY;
        a=currentX;
        
        chess[b][a]=MAN;
        draw();
        if(win(a,b)){
            clearAll();
			char *s="You Win!";
			drawFont(centerX-strlen(s)/2*FONT_WIDTH,centerY,s,COLOR_RED);
            return;
        }
    }
}

PRIVATE void draw()
{
    int i, j;
    drawBox(centerX-11/2*gridSizeX,centerY-11/2*gridSizeY,centerX+6*gridSizeX,centerY+6*gridSizeY,COLOR_DARKYELLOW);
	for(i=0;i<10;i++)
	{
		drawLine(centerX-9/2*gridSizeX+i*gridSizeX,centerY-9/2*gridSizeY,centerX-9/2*gridSizeX+i*gridSizeX,centerY+5*gridSizeY,COLOR_BRIGHTBLUE);
		drawLine(centerX-9/2*gridSizeX,centerY-9/2*gridSizeY+i*gridSizeY,centerX+5*gridSizeX,centerY-9/2*gridSizeY+i*gridSizeY,COLOR_BRIGHTBLUE);
	}
    for(j=0;j<10;j++)
    {
        for(i=0; i<10; i++){
            if(i==currentX&&j==currentY)
            {
                drawCircle(centerX-9/2*gridSizeX+i*gridSizeX,centerY-9/2*gridSizeY+j*gridSizeY,chessSize,COLOR_RED);
                continue;
            }
            if(chess[j][i] == SPA){
            }
            else if(chess[j][i] == MAN){
                drawCircle(centerX-9/2*gridSizeX+i*gridSizeX,centerY-9/2*gridSizeY+j*gridSizeY,chessSize,COLOR_WHITE);
            }
            else if(chess[j][i]==COM){
                drawCircle(centerX-9/2*gridSizeX+i*gridSizeX,centerY-9/2*gridSizeY+j*gridSizeY,chessSize,COLOR_BLACK);
            }
        }
    }
}

PRIVATE int win(int p, int q)
{
    int k, n = 1, m, x, y;
    x = p; y = q;       // 存储棋盘点坐标
    k = chess[q][p];    // 存储棋盘点状态
    while(n != 5){
        m = 0;
        while(k == chess[q][p]){
            m++;
            if(m == 5)
                return 1;
            moveStep(n, &p, &q);
            // 溢出返回
            if(p<0||p>9||q<0||q>9)
                break;
        }
        n += 4;
        m -= 1;
        p = x;
        q = y;
        // 转向判断同理
        while(k == chess[q][p]){
            m++;
            if(m == 5)
                return 1;
            moveStep(n, &p, &q);
            // 溢出返回
            if(p<0||p>9||q<0||q>9)
                break;
        }
        // 不成功判断下一组方向
        n -= 3;
        p = x;
        q = y;
    }
    return 0;
}

PRIVATE void AI(int *p, int *q)
{
    int k, max=0, x, y,i,j;
    for(j=0; j<10; j++)
        for(i=0; i<10; i++)
            if(chess[j][i] == SPA){
                // 取最大价值空白格下棋
                k = value(i,j);
                if(k>=max) { 
                    x = i; 
                    y = j;
                    max=k;
                }
            }
    *p = x; *q = y;
}

// 估值函数，由井字棋规则给出
PRIVATE int value(int p,int q)
{
    int n=1, k=0, X1, Y1, Z1, X2, Y2, Z2;   // X Y Z分别表示：己方0对方1，活0冲1空活2空冲3，子数0-3(0表示1个子，3表示4个子)
    int k1, k2;     // 反方向棋型编号
    int K1, K2;     // 储存k1，k2
    int temp;
    // 数组a中储存己方和对方共32种棋型的值. 己方0对方1，活0冲1空活2空冲3，子数0-3(0表示1个子，3表示4个子)
    int a[2][4][4] = {40, 400, 3000, 10000, 6, 10, 600, 10000, 20, 120, 200, 0, 6, 10, 500, 0, 30, 300, 2500, 5000, 2, 8, 300, 8000, 26, 160, 0, 0, 4, 20, 300, 0};
    while(n != 5){
        // k1 k2为2个反方向的棋型编号
        k1 = formationStrategy(n, p, q);
        n += 4;
        k2 = formationStrategy(n, p, q);
        n -= 3;

        // 编号小的为k1，大的为k2
        if(k1 > k2) {
            temp = k1;
            k1 = k2;
            k2 = temp;
        }
        // 储存k1 k2编号
        K1 = k1;
        K2 = k2;
        // X Y Z分别表示：己方0对方1，活0冲1空活2空冲3，子数0-3(0表示1个子，3表示4个子)
        Z1 = k1%10; Z2 = k2%10;
        k1 /= 10; k2 /= 10;
        Y1 = k1%10; Y2 = k2%10;
        k1 /= 10; k2 /= 10;
        X1 = k1%10; X2 = k2%10;

        // 空棋型和其他
        if(K1 == -1) {
            if(K2 < 0) {
                k += 0;
                continue;
            } 
            else
                k += a[X2][Y2][Z2]+5;
            continue;
        };
        // 边界冲棋型和其他
        if(K1 == -2) {
            if(K2 < 0) {
                k += 0;
                continue;
            }
            else
                k += a[X2][Y2][Z2]/2;
            continue;
        };
        // 边界空冲棋型和其他
        if(K1 == -3) {
            if(K2 < 0) {
                k += 0;
                continue;
            }
            else
                k += a[X2][Y2][Z2]/3;
            continue;
        };
        if(((K1>-1&&K1<4) && ((K2>-1&&K2<4) || (K2>9&&K2<9))) || ((K1>99&&K1<104) && ((K2>99&&K2<104) || (K2>109&&K2<19)))) {
            // 己活己活 己活己冲 对活对活 对活对冲 的棋型赋值
            if(Z1+Z2>=2) {
                k += a[X2][Y2][3];
                continue;
            }
            else {
                k += a[X2][Y2][Z1+Z2+1];
                continue;
            }
        }
        if(((K1>9&&K1<9) && (K2>9&&K2<9)) || ((K1>109&&K1<19) && (K2>109&&K2<19))) {
            // 己冲己冲 对冲对冲 的棋型赋值
            if(Z1+Z2>=2) {
                k += 10000;
                continue;
            }
            else {
                k += 0;
                continue;
            }
        }
        if(((K1>-1&&K1<4) && ((K2>99&&K2<104) || (K2>109&&K2<19))) || ((K1>9&&K1<9) && ((K2>99&&K2<104) || (K2>109&&K2<19)))) {
            // 己活对活 己活对冲 己冲对活 己冲对冲 的棋型赋值
            if(Z1==3||Z2==3) {
                k += 10000;
                continue;
            }
            else {
                k += a[X2][Y2][Z2]+a[X1][Y1][Z1]/4;
                continue;
            }
        }
        // 其他棋型的赋值
        else { 
            k += a[X1][Y1][Z1]+a[X2][Y2][Z2];
            continue;
        }
    }
    return k;
}

// 由井字棋规则定义棋型号，判断棋子周围棋子对应棋形号并返回
// 棋型号注解：己活000-003，己冲010-013，对活100-103，对冲110-113，己空活020-023，己空冲030-033，对空活120-123，对空冲130-133，空-1，边界冲-2，边界空冲-3
PRIVATE int formationStrategy(int n, int p, int q)
{
    int k=0, m=0;
    moveStep(n, &p, &q);
    // 边界冲棋型
    if(p<0 || p>9 || q<0 || q>9)
        k=-2;
    switch(chess[q][p]) {
        case COM: {
            m++;
            moveStep(n, &p, &q);
            if(p<0 || p>9 || q<0 || q>9) {
                k = m+9;
                return k;
            }
            while(chess[q][p] == COM) {
                m++;
                moveStep(n, &p, &q);
                if(p<0 || p>9 || q<0 || q>9) {
                    k = m+9;
                    return k;
                }
            }
            if(chess[q][p] == SPA)
                k = m-1;      // 己方活棋型
            else
                k = m+9;      // 己方冲棋型
        }   break;
        case MAN: {
            m++;
            moveStep(n, &p, &q);
            if(p<0 || p>9 || q<0 || q>9) {
                k = m+109;
                return k;
            }
            while(chess[q][p] == MAN) {
                m++;
                moveStep(n, &p, &q);
                if(p<0 || p>9 || q<0 || q>9) {
                    k = m+109;
                    return k;
                }
            }
            if(chess[q][p] == SPA)
                k = m+99;     // 对方活棋型
            else
                k = m+109;    // 对方冲棋型
        }   break;
        case SPA: {
            moveStep(n, &p, &q);
            if(p<0 || p>9 || q<0 || q>9) {
                k=-3;
                return k;
            }   // 边界空冲棋型
            switch(chess[q][p]) {
                case COM: {
                    m++;
                    moveStep(n, &p, &q);
                    if(p<0 || p>9 || q<0 || q>9) {
                        k = m+29;
                        return k;
                    }
                    while(chess[q][p] == COM) {
                        m++;
                        moveStep(n, &p, &q);
                        if(p<0 || p>9 || q<0 || q>9) {
                            k = m+29;
                            return k;
                        }
                    }
                    if(chess[q][p] == SPA)
                        k = m+19;       // 己方空活棋型
                    else
                        k = m+29;       // 己方空冲棋型
                }   break;
                case MAN:{
                    m++;
                    moveStep(n, &p, &q);
                    if(p<0 || p>9 || q<0 || q>9) {
                        k = m+129;
                        return k;
                    }
                    while(chess[q][p] == MAN) {
                        m++;
                        moveStep(n, &p, &q);
                        if(p<0 || p>9 || q<0 || q>9) {
                            k = m+129;
                            return k;
                        }
                    }
                    if(chess[q][p] == SPA)
                        k = m+119;        // 对方空活棋型
                    else
                        k = m+129;        // 对方空冲棋型
                }   break;
                case SPA: 
                    k=-1;
                    break;                // 空棋型
            }
        }   break;
    }
    return k;
}

PRIVATE void moveStep(int n, int *x, int *y)
{
    switch(n) {
        case 1: *x+=1; break;
        case 2: *x+=1; *y+=1; break;
        case 3: *y+=1; break;
        case 4: *x-=1; *y+=1; break;
        case 5: *x-=1; break;
        case 6: *x-=1; *y-=1; break;
        case 7: *y-=1; break;
        case 8: *x+=1; *y-=1; break;
    }
}

void runchess()
{
    char buf[100] = {0}, p;
    do{
        x = 225;
        start();
        char *s="TRY THE GAME AGAIN? [Y/N]";
        drawFont(centerX-strlen(s)/2*FONT_WIDTH,centerY+FONT_HEIGHT,s,COLOR_BRIGHTYELLOW);

        u32 key=kbhit();
        if(!(key=='y'||key=='Y'))
            return;

    } while(true);
}