/*************************************************************************//**
 *****************************************************************************
 * @file   topBar.c
 * @brief  topBar()
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
 *                                topBar
 *****************************************************************************/
PUBLIC int topBar(const char *user)
{
	MESSAGE msg;

	msg.type	= TOPBAR;

	msg.PATHNAME	= (void*)user;
	msg.NAME_LEN	= strlen(user);

	send_recv(BOTH, TASK_GRAPHICS, &msg);

	return msg.RETVAL;;
}

PUBLIC int do_topBar()
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




