/*************************************************************************//**
 *****************************************************************************
 * @file   drawBMP.c
 * @brief  drawBMP()
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

PUBLIC int do_drawBMP()
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




