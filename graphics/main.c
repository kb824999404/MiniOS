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
            // graphics_msg.RETVAL=do_topBar();
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
