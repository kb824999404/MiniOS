
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                            graphics.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
													Kaibin Zhou,2020
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
#include "graphics.h"
#include "stdio.h"
#include "font.h"
#include "string.h"
unsigned char table_rgb[16 * 3] = {
		0x00, 0x00, 0x00,	/*  0:黑 */
		0xff, 0x00, 0x00,	/*  1:梁红 */
		0x00, 0xff, 0x00,	/*  2:亮绿 */
		0xff, 0xff, 0x00,	/*  3:亮黄 */
		0x00, 0x00, 0xff,	/*  4:亮蓝 */
		0xff, 0x00, 0xff,	/*  5:亮紫 */
		0x64, 0x95, 0xED,	/*  6:浅亮蓝 */
		0xff, 0xff, 0xff,	/*  7:白 */
		0xdd, 0xdd, 0xdd,	/*  8:亮灰 */
		0x84, 0x00, 0x00,	/*  9:暗红 */
		0x00, 0x84, 0x00,	/* 10:暗绿 */
		0xCD, 0xBE, 0x70,	/* 11:暗黄 */
		0x00, 0x00, 0x84,	/* 12:暗青 */
		0x84, 0x00, 0x84,	/* 13:暗紫 */
		0x87, 0xCE, 0xFA,   /* 14:浅暗蓝 */
		0x84, 0x84, 0x84	/* 15:暗灰 */
};
unsigned char table_gray[16 * 3] = {
		0x00,0x00,0x00,
        0x11,0x11,0x11,
        0x22,0x22,0x22,
        0x33,0x33,0x33,
        0x44,0x44,0x44,
        0x55,0x55,0x55,
        0x66,0x66,0x66,
        0x77,0x77,0x77,
        0x88,0x88,0x88,
        0x99,0x99,0x99,
        0xaa,0xaa,0xaa,
        0xbb,0xbb,0xbb,
        0xcc,0xcc,0xcc,
        0xdd,0xdd,0xdd,
        0xee,0xee,0xee,
        0xff,0xff,0xee,
};
int colorMode=1;
//初始化图形界面
void init_graphics(void)
{
	char s[40];
	int i;

	init_palette();/* 设定调色板 */
	clearScreen();
}


void init_palette(void)
{
	set_palette(0, 15, table_rgb);
	return;

	/* C语言中的static char语句只能用于数据，相当于汇编中的DB指令 */
}
PUBLIC void grayPalette()
{
	set_palette(0, 15, table_gray);
	return;
}
PUBLIC void colorPalette()
{
	set_palette(0, 15, table_rgb);
	return;	
}

PUBLIC void clearScreen()
{
	boxfill8(SCREEN_WIDTH,DEFAULT_BACKGROUND,0,0,SCREEN_WIDTH,SCREEN_HEIGHT);	
}

PUBLIC int readBMP(char* filename)
{
    char buff[512];
    char *p=VGA_MEM_BASE;
    int fd = open(filename, O_RDWR),n,j,i,buffSize=512;
	if(fd==-1)
	{
		return 0;
	}
    for(i=0;i<BMP_HEAD_SIZE/buffSize;i++)
    {
        read(fd,buff,buffSize);
    }

    read(fd,buff,BMP_HEAD_SIZE%buffSize);

    for(i=0;i<SCREEN_SIZE/buffSize;i++)
    {
        n=read(fd,buff,buffSize);
        for(j=0;j<n;j++)
		{
			buff[j]%=16;
			if(buff[j]<0)	buff[j]+=16;
            p[j]=buff[j];

		}
        p+=n;
    }

    n=read(fd,buff,SCREEN_SIZE%buffSize);
    for(j=0;j<n;j++)
        p[j]=buff[j];
    close(fd);
    return 1;
}

void set_palette(int start, int end, unsigned char *rgb)
{
	int i, eflags;
	eflags = io_load_eflags();	/* 记录中断许可标志的值 */
	io_cli(); 					/* 将中断许可标志置为0,禁止中断 */
	io_out8(0x03c8, start);
	for (i = start; i <= end; i++) {
		io_out8(0x03c9, rgb[0] / 4);
		io_out8(0x03c9, rgb[1] / 4);
		io_out8(0x03c9, rgb[2] / 4);
		rgb += 3;
	}
	io_store_eflags(eflags);	/* 复原中断许可标志 */
	return;
}

PUBLIC void boxfill8(int xsize, unsigned char c, int x0, int y0, int x1, int y1)
{
	int x, y,pos;
	char *p=VGA_MEM_BASE;
	for (y = y0; y <= y1; y++) {
		for (x = x0; x <= x1; x++)
		{
			pos=y * xsize + x;
			if(pos>=SCREEN_SIZE) continue;
			p[pos]=c;
		}
	}
	return;
}

//重画顶部栏
PUBLIC  void updateTopbar(char *user)
{
	char s[40];
	sprintf(s,"Hello!%s ",user);
	boxfill8(SCREEN_WIDTH,COLOR_BRIGHTGRAY,0,0,SCREEN_WIDTH,TOPBAR_HEIGHT);
	putfonts8_asc( FONT_WIDTH,  4, 6, "MINIOS");
	putfonts8_asc( SCREEN_WIDTH-strlen(s)*FONT_WIDTH,4, COLOR_WHITE, s);
}

//情况文字部分
PUBLIC void clearFonts()
{
	boxfill8(SCREEN_WIDTH,DEFAULT_BACKGROUND,0,TOPBAR_HEIGHT,SCREEN_WIDTH,SCREEN_HEIGHT);

}

PUBLIC void putfont8(int x, int y, char c, char *font)
{
	int i;
	char *p, d /* data */;
	for (i = 0; i < 16; i++) {
		p = VGA_MEM_BASE + (y + i) *SCREEN_WIDTH + x;
		d = font[i];
		if ((d & 0x80) != 0) { p[0] = c; }
		if ((d & 0x40) != 0) { p[1] = c; }
		if ((d & 0x20) != 0) { p[2] = c; }
		if ((d & 0x10) != 0) { p[3] = c; }
		if ((d & 0x08) != 0) { p[4] = c; }
		if ((d & 0x04) != 0) { p[5] = c; }
		if ((d & 0x02) != 0) { p[6] = c; }
		if ((d & 0x01) != 0) { p[7] = c; }
	}
	return;
}
PUBLIC void putfonts8_char(int x, int y, char c, char ch)
{
	putfont8(x, y, c, hankaku + ch * 16);
}

PUBLIC void putfonts8_asc(int x, int y, char c, unsigned char *s)
{
	for (; *s != 0x00; s++) {
		putfont8(x, y, c, hankaku + *s * 16);
		x += FONT_WIDTH;
	}
	return;
}

PUBLIC int pixel(int x,int y,unsigned char c)
{
	char *p=VGA_MEM_BASE;
	if(x<0||y<0)
		return 0;
	int pos=y*SCREEN_WIDTH+x;
	if(pos<SCREEN_SIZE)
	{	
		p[pos]=c;
		return 1;
	}
	else
		return 0;
}