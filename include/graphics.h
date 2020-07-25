/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                            graphics.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                                                    Kaibin Zhou, 2020
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
#ifndef	_MINIOS_GRAPHICS_H_
#define	_MINIOS_GRAPHICS_H_

#define COL8_000000		0     /*  0:黑 */
#define COL8_FF0000		1     /*  1:梁红 */
#define COL8_00FF00		2     /*  2:亮绿 */
#define COL8_FFFF00		3     /*  3:亮黄 */
#define COL8_0000FF		4     /*  4:亮蓝 */
#define COL8_FF00FF		5     /*  5:亮紫 */
#define COL8_00FFFF		6     /*  6:浅亮蓝 */
#define COL8_FFFFFF		7     /*  7:白 */
#define COL8_C6C6C6		8     /*  8:亮灰 */
#define COL8_840000		9     /*  9:暗红 */
#define COL8_008400		10    /* 10:暗绿 */
#define COL8_848400		11    /* 11:暗黄 */
#define COL8_000084		12    /* 12:暗青 */
#define COL8_840084		13    /* 13:暗紫 */
#define COL8_008484		14    /* 14:浅暗蓝 */
#define COL8_848484		15    /* 15:暗灰 */

#define COLOR_BLACK 0  /*  0:黑 */
#define COLOR_RED 1  /*  1:梁红 */
#define COLOR_BRIGHTGREEN 2  /*  2:亮绿 */
#define COLOR_BRIGHTYELLOW 3	 /*  3:亮黄 */
#define COLOR_BRIGHTBLUE 4	 /*  4:亮蓝 */
#define COLOR_BRIGHTPURPER 5 /*  5:亮紫 */
#define COLOR_BRIGHTBLUESHALLOW 6/*  6:浅亮蓝 */
#define COLOR_WHITE 7 /*  7:白 */
#define COLOR_BRIGHTGRAY 8	 /*  8:亮灰 */
#define COLOR_DARKRED 9 /*  9:暗红 */
#define COLOR_DARKGREEN 10 /* 10:暗绿 */
#define COLOR_DARKYELLOW 11	 /* 11:暗黄 */
#define COLOR_DARKBLUEGREEN 12 /* 12:暗青 */
#define COLOR_DARKPURPER 13	 /* 13:暗紫 */
#define COLOR_DARKBLUESHALLOW 14 /* 14:浅暗蓝 */
#define COLOR_DARKGRAY 15 /* 15:暗灰 */

#define DEFAULT_BACKGROUND COLOR_DARKBLUESHALLOW //默认背景色
#define DEFAULT_FRONT COLOR_WHITE       //默认前景色
#define DEFAULT_SHADOW COLOR_BLACK		//默认阴影色

#define SCREEN_WIDTH 320			//屏幕宽度
#define SCREEN_HEIGHT 200			//屏幕高度
#define SCREEN_SIZE SCREEN_WIDTH*SCREEN_HEIGHT
#define FONT_WIDTH 8				//字体宽度
#define FONT_HEIGHT 16				//字体高度

#define TOPBAR_HEIGHT (8+FONT_HEIGHT) //顶部高度
#define ROW_NUM 40					//列数
#define LINE_NUM 12					//行数

#define	VGA_MEM_BASE	0xa0000
#define VGA_MEM_FONT VGA_MEM_BASE+(TOPBAR_HEIGHT*SCREEN_WIDTH)

#define BMP_HEAD_SIZE 0x432

#include "type.h"
/* lib/graphics.c*/
PUBLIC void clearAll();
PUBLIC int drawBMP(const char *filename);
PUBLIC int topBar(const char *user);
PUBLIC void drawFont(int x, int y, char *s,char c);
PUBLIC int drawPixel( int x, int y,unsigned char c);
PUBLIC void drawBox( int x0, int y0, int x1, int y1,unsigned char c);
PUBLIC void drawLine(int x0, int y0, int x1, int y1,unsigned char c);
PUBLIC void drawCircle( int x, int y,int radius,unsigned char c);

/* graphics/graphics.c*/
PUBLIC void clearScreen();
PUBLIC int readBMP(char* filename);
PUBLIC void boxfill8(int xsize, unsigned char c, int x0, int y0, int x1, int y1);
PUBLIC  void updateTopbar(char *user);
PUBLIC void clearFonts();
PUBLIC void putfont8(int x, int y, char c, char *font);
PUBLIC void putfonts8_char(int x, int y, char c, char ch);
PUBLIC void putfonts8_asc(int x, int y, char c, unsigned char *s);
PUBLIC int pixel(int x,int y,unsigned char c);
PUBLIC void grayPalette();
PUBLIC void colorPalette();

extern unsigned char table_rgb[16 * 3];

extern unsigned char table_gray[16 * 3];

extern int colorMode;

#endif /* _MINIOS_GRAPHICS_H_ */