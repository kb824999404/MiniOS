/*************************************************************************//**
 *****************************************************************************
 * @file   draw.c
 * @author Kaibin Zhou
 * @date   2020
 *****************************************************************************
 *****************************************************************************/

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

/*****************************************************************************
 *                                drawBMP
 *****************************************************************************/
PUBLIC int drawBMP(const char *filename)
{
	MESSAGE msg;

	msg.type	= DRAW;

	msg.PATHNAME	= (void*)filename;
	msg.NAME_LEN	= strlen(filename);

	send_recv(BOTH, TASK_GRAPHICS, &msg);

	return msg.RETVAL;;
}

/*****************************************************************************
 *                                topBar
 *****************************************************************************/
PUBLIC int topBar(const char *user)
{
	MESSAGE msg;

	msg.type	= TOPBAR;

	msg.PATHNAME	= (void*)user;
	msg.NAME_LEN	= strlen(user);

	send_recv(BOTH, TASK_GRAPHICS, &msg);

	return msg.RETVAL;
}
/*****************************************************************************
 *                                clearAll
 *****************************************************************************/
PUBLIC void clearAll()
{
	MESSAGE msg;
	msg.type	= CLEAR;
	send_recv(BOTH, TASK_GRAPHICS, &msg);
}
/*****************************************************************************
 *                                changeColorMode
 *****************************************************************************/
PUBLIC void changeColorMode()
{
	MESSAGE msg;
	msg.type	= MODE;
	send_recv(BOTH, TASK_GRAPHICS, &msg);
}
/*****************************************************************************
 *                                drawFont
 *****************************************************************************/
PUBLIC void drawFont(int x, int y, char *s,char c)
{
	MESSAGE msg;
	msg.type	= FONT;
	msg.OFFSETX=x;
	msg.OFFSETY=y;
	msg.COLOR3=c;
	msg.STRING	= (void*)s;
	msg.STRLEN	= strlen(s);

	send_recv(BOTH, TASK_GRAPHICS, &msg);

}
/*****************************************************************************
 *                                drawPixel
 *****************************************************************************/
PUBLIC int drawPixel( int x, int y,unsigned char c)
{
	MESSAGE msg;
	msg.type	= PIXEL;
	msg.M1I1=c;
	msg.M1I2=x;
	msg.M1I3=y;
	send_recv(BOTH, TASK_GRAPHICS, &msg);
	return msg.RETVAL;

}
/*****************************************************************************
 *                                drawBox
 *****************************************************************************/
PUBLIC void drawBox( int x0, int y0, int x1, int y1,unsigned char c)
{
	MESSAGE msg;
	msg.type	= BOX;
	msg.COLOR4=c;
	msg.X1=x0;
	msg.Y1=y0;
	msg.X2=x1;
	msg.Y2=y1;
	send_recv(BOTH, TASK_GRAPHICS, &msg);

}
/*****************************************************************************
 *                                drawLine
 *****************************************************************************/
PUBLIC void drawLine(int x0, int y0, int x1, int y1,unsigned char c)
{
	MESSAGE msg;
	msg.type	= LINE;
	msg.COLOR4=c;
	msg.X1=x0;
	msg.Y1=y0;
	msg.X2=x1;
	msg.Y2=y1;
	send_recv(BOTH, TASK_GRAPHICS, &msg);

}
/*****************************************************************************
 *                                drawCircle
 *****************************************************************************/
PUBLIC void drawCircle( int x, int y,int radius,unsigned char c)
{
	MESSAGE msg;
	msg.type	= CIRCLE;
	msg.M1I1=c;
	msg.M1I2=x;
	msg.M1I3=y;
	msg.M1I4=radius;
	send_recv(BOTH, TASK_GRAPHICS, &msg);

}