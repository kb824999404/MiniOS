/*************************************************************************//**
 *****************************************************************************
 * @file   draw.c
 * @author Kaibin Zhou
 * @date   2020
 *****************************************************************************
 *****************************************************************************/

#include "stdio.h"
#include "string.h"
#include "graphics.h"

#define PANELSIZE (SCREEN_HEIGHT-TOPBAR_HEIGHT-FONT_HEIGHT)
int centerX=SCREEN_WIDTH/2,centerY=PANELSIZE/2+TOPBAR_HEIGHT;
int sizeX=SCREEN_WIDTH/2-FONT_WIDTH/2;
int sizeY=PANELSIZE/2-FONT_HEIGHT/2;

int main(int argc, char * argv[])
{

    clearAll();
    drawPanel();
    drawGrid();
    boxTest();
    circleTest();
    fontTest();
    smille();
    return 0;
}

/*绘制画布*/
void drawPanel()
{
    int shadowSize=2;
    drawBox(centerX-sizeX+shadowSize,centerY-sizeY+shadowSize,
    centerX+sizeX+shadowSize,centerY+sizeY+shadowSize,COLOR_BLACK);
    drawBox(centerX-sizeX,centerY-sizeY,centerX+sizeX,centerY+sizeY,COLOR_WHITE);
}
/*绘制网格*/
void drawGrid()
{
    drawLine(centerX,centerY-sizeY,centerX,centerY+sizeY,COLOR_BRIGHTBLUESHALLOW);
    drawLine(centerX-sizeX,centerY,centerX+sizeX,centerY,COLOR_BRIGHTBLUESHALLOW);
}
/*绘制长方体*/
void boxTest()
{
    int cx=centerX-sizeX/2;
    int cy=centerY-sizeY/2;
    int size=30;
    drawBox(cx-size,cy-size,cx+size,cy+size,COLOR_DARKYELLOW);
}
/*绘制圆*/
void circleTest()
{
    int cx=centerX+sizeX/2;
    int cy=centerY-sizeY/2;
    int radius=30;
    drawCircle(cx,cy,radius,COLOR_DARKBLUEGREEN);
}
/*绘制文字*/
void fontTest()
{
    int cx=centerX-sizeX/2;
    int cy=centerY+sizeY/2;
    char s[]="Hello!";
    drawFont(cx-FONT_WIDTH*strlen(s)/2+1,cy-FONT_HEIGHT/2+1,s,COLOR_BLACK);
    drawFont(cx-FONT_WIDTH*strlen(s)/2,cy-FONT_HEIGHT/2,s,COLOR_BRIGHTGRAY);
}
/*绘制笑脸*/
void smille()
{
    int cx=centerX+sizeX/2;
    int cy=centerY+sizeY/2;
    int radius=30;
    /*脸*/
    drawCircle(cx,cy,radius,COLOR_BRIGHTYELLOW);
    radius=8;
    /*眼白*/
    drawCircle(cx-10,cy-10,radius,COLOR_WHITE);
    drawCircle(cx+10,cy-10,radius,COLOR_WHITE);
    /*嘴*/
    drawPixel(cx,cy+15,COLOR_BLACK);
    int i,len=4;
    for(i=1;i<=len;i++)
    {
        drawPixel(cx-i,cy+15,COLOR_BLACK);
        drawPixel(cx+i,cy+15,COLOR_BLACK);
    }
    for(i=1;i<=3;i++)
    {
        drawPixel(cx-(len+i),cy+15-i,COLOR_BLACK);
        drawPixel(cx+(len+i),cy+15-i,COLOR_BLACK);
    }
    /*眼黑*/
    drawBox(cx-10-4-1,cy-10-1,cx-10-4+1,cy-10,COLOR_BLACK);
    drawBox(cx+10-4-1,cy-10-1,cx+10-4+1,cy-10,COLOR_BLACK);
    for(i=1;i<=8;i++)
    {
        delay(10);
        drawBox(cx-10-4-1+i-1,cy-10-1,cx-10-4+1+i-1,cy-10+1,COLOR_WHITE);
        drawBox(cx+10-4-1+i-1,cy-10-1,cx+10-4+1+i-1,cy-10+1,COLOR_WHITE);

        drawBox(cx-10-4-1+i,cy-10-1,cx-10-4+1+i,cy-10+1,COLOR_BLACK);
        drawBox(cx+10-4-1+i,cy-10-1,cx+10-4+1+i,cy-10+1,COLOR_BLACK);
    }
    for(i=8;i>=1;i--)
    {
        delay(10);
        drawBox(cx-10-4-1+i,cy-10-1,cx-10-4+1+i,cy-10+1,COLOR_WHITE);
        drawBox(cx+10-4-1+i,cy-10-1,cx+10-4+1+i,cy-10+1,COLOR_WHITE);

        drawBox(cx-10-4-1+i-1,cy-10-1,cx-10-4+1+i-1,cy-10+1,COLOR_BLACK);
        drawBox(cx+10-4-1+i-1,cy-10-1,cx+10-4+1+i-1,cy-10+1,COLOR_BLACK);
    }

}
      
void delay(int time)
{
	int i, j, k;
	for(k=0;k<time;k++){
		/*for(i=0;i<10000;i++){	for Virtual PC	*/
		for(i=0;i<10;i++){/*	for Bochs	*/
			for(j=0;j<10000;j++){}
		}
	}
}