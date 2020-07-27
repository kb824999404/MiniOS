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
 *                                kbhit
 *****************************************************************************/
PUBLIC u32 kbhit()
{
	MESSAGE msg;

	msg.type	= KBHIT;

	msg.M1I1 =0;

	send_recv(BOTH, TASK_TTY, &msg);

	return msg.RETVAL;;
}
