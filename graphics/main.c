/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                            main.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
													Kaibin Zhou,2020
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
#include "graphics.h"
#include "type.h"
#include "config.h"
#include "stdio.h"
#include "const.h"
#include "protect.h"
#include "string.h"
#include "fs.h"
#include "proc.h"
#include "tty.h"
#include "console.h"
#include "global.h"
#include "keyboard.h"
#include "proto.h"
int colorMode;

PUBLIC void task_graphics()
{
	printl("{GRAPHICS} Task GRAPHICS begins.\n");

	while (1) {
		send_recv(RECEIVE, ANY, &graphics_msg);
		int src = graphics_msg.source;
		int reply = 1;

		int msgtype = graphics_msg.type;

		switch (msgtype) {
        case DRAW:
            graphics_msg.RETVAL=do_drawBMP();
			break;
		case TOPBAR:
            graphics_msg.RETVAL=do_topBar();
			break;
		case CLEAR:
            do_clear();
			break;	
		case PIXEL:
			graphics_msg.RETVAL=do_drawPixel();
			break;
		case FONT:
			do_drawFont();
			break;
		case BOX:
			do_drawBox();
			break;
		case CIRCLE:
			do_drawCircle();
			break;
		case LINE:	
			do_drawLine();
			break;
		case MODE:
			do_changeColorMode();
			break;
		default:
			dump_msg("GRAPHICS::unknown msg", &graphics_msg);
			assert(0);
			break;
		}

		if (reply) {
			graphics_msg.type = SYSCALL_RET;
			send_recv(SEND, src, &graphics_msg);
		}
	}
}


int do_drawBMP()
{
	char pathname[MAX_PATH];

	int name_len =  graphics_msg.NAME_LEN;	/* length of filename */
	int src = graphics_msg.source;	/* caller proc nr. */
	assert(name_len < MAX_PATH);
	phys_copy((void*)va2la(TASK_GRAPHICS, pathname),
		  (void*)va2la(src, graphics_msg.PATHNAME),
		  name_len);
	pathname[name_len] = 0;

    return readBMP(pathname);
}


int do_topBar()
{
	char user[MAX_PATH];

	int name_len =  graphics_msg.NAME_LEN;	/* length of filename */
	int src = graphics_msg.source;	/* caller proc nr. */
	assert(name_len < MAX_PATH);
	phys_copy((void*)va2la(TASK_GRAPHICS, user),
		  (void*)va2la(src, graphics_msg.PATHNAME),
		  name_len);
	user[name_len] = 0;

    updateTopbar(user);
    return 1;
}

void do_clear()
{
	clearScreen();
}

/*****************************************************************************
 *                                drawFont
 *****************************************************************************/
void do_drawFont()
{
	unsigned char str[MAX_PATH];

	int str_len =  graphics_msg.STRLEN;	/* length of filename */
	int src = graphics_msg.source;	/* caller proc nr. */
	assert(str_len < MAX_PATH);
	phys_copy((void*)va2la(TASK_GRAPHICS, str),
		  (void*)va2la(src, graphics_msg.STRING),
		  str_len);
	str[str_len] = 0;
	putfonts8_asc(graphics_msg.OFFSETX, graphics_msg.OFFSETY, graphics_msg.COLOR3, str);

}
/*****************************************************************************
 *                                drawPixel
 *****************************************************************************/
int do_drawPixel()
{
	unsigned char c=graphics_msg.M1I1;
	int x=graphics_msg.M1I2;
	int y=graphics_msg.M1I3;
	return pixel(x,y,c);

}
/*****************************************************************************
 *                                drawBox
 *****************************************************************************/
void do_drawBox()
{
	boxfill8(SCREEN_WIDTH,graphics_msg.COLOR4,graphics_msg.X1,graphics_msg.Y1,
	graphics_msg.X2,graphics_msg.Y2);

}
/*****************************************************************************
 *                                drawLine
 *****************************************************************************/
void do_drawLine()
{
	unsigned char c=graphics_msg.COLOR4;
	int x1=graphics_msg.X1;
	int y1=graphics_msg.Y1;
	int x2=graphics_msg.X2;
	int y2=graphics_msg.Y2;
	int i,j;
	if(x1==x2)		//竖线
	{
		if(y1<y2)
		{
			for(i=y1;i<=y2;i++)
				pixel(x1,i,c);
		}
		else
		{
			for(i=y2;i<=y1;i++)
				pixel(x1,i,c);	
		}
		return;
	}
	else if(y1==y2)	//横线
	{
		if(x1<x2)
		{
			for(i=x1;i<=x2;i++)
				pixel(i,y1,c);
		}
		else
		{
			for(i=x2;i<=x1;i++)
				pixel(i,y1,c);
		}		
		return;
	}
	else			//斜线
	{
		return;
	}
	

}

/*****************************************************************************
 *                                drawCircle
 *****************************************************************************/
void do_drawCircle()
{
	unsigned char c=graphics_msg.M1I1;
	int x=graphics_msg.M1I2;
	int y=graphics_msg.M1I3;
	int radius=graphics_msg.M1I4;
	int l=x-radius;
	int r=x+radius;
	int u=y-radius;
	int d=y+radius;
	l=(l>=0)?l:0;
	r=(r<SCREEN_WIDTH)?r:SCREEN_WIDTH-1;
	u=(u>=0)?u:0;
	d=(d<SCREEN_HEIGHT)?d:SCREEN_HEIGHT-1;
	int i,j;
	for(i=l;i<=r;i++)
	{
		for(j=u;j<=d;j++)
		{
			int dis=(i-x)*(i-x)+(j-y)*(j-y);
			if(dis<=radius*radius)
				pixel(i,j,c);
		}
	}
}

void do_changeColorMode()
{
	if(colorMode==1)
	{
		grayPalette();
		colorMode=0;
	}
	else
	{
		colorPalette();
		colorMode=1;
	}
	
}

