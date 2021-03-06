/*************************************************************************//**
 *****************************************************************************
 * @file   console.c
 * @brief  Manipulate the console.
 * @author Forrest Y. Yu
 * @date   2005
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
#include "keyboard.h"
#include "proto.h"
#include "graphics.h"

/* #define __TTY_DEBUG__ */

/* local routines */
PRIVATE void	set_cursor(unsigned int position);
PRIVATE void	set_video_start_addr(u32 addr);
PRIVATE void	flush(CONSOLE* con);
PRIVATE	void	w_copy(CONSOLE *con,unsigned int dst,unsigned int src);
PRIVATE void	clear_screen(int pos, int len);

/*****************************************************************************
 *                                init_screen
 *****************************************************************************/
/**
 * Initialize the console of a certain tty.
 * 
 * @param tty  Whose console is to be initialized.
 *****************************************************************************/
PUBLIC void init_screen(TTY* tty)
{
	int nr_tty = tty - tty_table;
	// tty->console = console_table + nr_tty;
	tty->console = console_table;

	/* 
	 * NOTE:
	 *   variables related to `position' and `size' below are
	 *   in WORDs, but not in BYTEs.
	 */
	// int v_mem_size = V_MEM_SIZE >> 1; /* size of Video Memory */
	// int size_per_con = v_mem_size / NR_CONSOLES;
	// tty->console->orig = nr_tty * size_per_con;
	// tty->console->con_size = size_per_con / SCR_WIDTH * SCR_WIDTH;
	// tty->console->cursor = tty->console->crtc_start = tty->console->orig;
	// tty->console->is_full = 0;

	int v_mem_size = V_MEM_SIZE >> 1; /* size of Video Memory */
	int size_per_con = v_mem_size / NR_CONSOLES;
	tty->console->orig = 0;
	tty->console->con_size = SCR_SIZE;
	tty->console->cursor = tty->console->crtc_start = tty->console->orig;
	tty->console->is_full = 0;

	if (nr_tty == 0) {
		// tty->console->cursor = disp_pos / 2;
		disp_pos = 0;
	}
	else {
		/* 
		 * `?' in this string will be replaced with 0, 1, 2, ...
		 */
		const char prompt[] = "[TTY #?]\n";

		const char * p = prompt;
		for (; *p; p++)
			out_char(tty->console, *p == '?' ? nr_tty + '0' : *p);
	}

	set_cursor(tty->console->cursor);
}


/*****************************************************************************
 *                                out_char
 *****************************************************************************/
/**
 * Print a char in a certain console.
 * 
 * @param con  The console to which the char is printed.
 * @param ch   The char to print.
 *****************************************************************************/
PUBLIC void out_char(CONSOLE* con, char ch)
{
	int i;
	// u8* pch = (u8*)(V_MEM_BASE + con->cursor * 2);

	assert(con->cursor - con->orig < con->con_size);

	/*
	 * calculate the coordinate of cursor in current console (not in
	 * current screen)
	 */
	int cursor_x = (con->cursor - con->orig) % SCR_WIDTH;
	int cursor_y = (con->cursor - con->orig) / SCR_WIDTH;

	switch(ch) {
	case '\n':
		con->cursor = con->orig + SCR_WIDTH * (cursor_y + 1);
		break;
	case '\b':
		if (con->cursor > con->orig) {
			con->cursor--;
			cursor_x = (con->cursor - con->orig) % SCR_WIDTH;
			cursor_y = (con->cursor - con->orig) / SCR_WIDTH;
			con->cbuf[con->cursor]=ch;
			putfont(cursor_x,cursor_y,ch);
			// *(pch - 2) = ' ';
			// *(pch - 1) = DEFAULT_CHAR_COLOR;

		}
		break;
	case '\t':
		for (i = 0; i < TAB_WIDTH; i++) {
			// *pch++ = ' ';
			// *pch++ = DEFAULT_CHAR_COLOR;
			cursor_x = (con->cursor - con->orig) % SCR_WIDTH;
			cursor_y = (con->cursor - con->orig) / SCR_WIDTH;
			con->cbuf[con->cursor]=ch;
			putfont(cursor_x,cursor_y,ch);
			con->cursor++;
		}
		break;
	default:
		// *pch++ = ch;
		// *pch++ = DEFAULT_CHAR_COLOR;
		cursor_x = (con->cursor - con->orig) % SCR_WIDTH;
		cursor_y = (con->cursor - con->orig) / SCR_WIDTH;
		con->cbuf[con->cursor]=ch;
		putfont(cursor_x,cursor_y,ch);		
		con->cursor++;
		break;
	}

	if (con->cursor - con->orig >= con->con_size) {
		cursor_x = (con->cursor - con->orig) % SCR_WIDTH;
		cursor_y = (con->cursor - con->orig) / SCR_WIDTH;
		// int cp_orig = (cursor_y + 1) * SCR_WIDTH - SCR_SIZE;
		int cp_orig = SCR_WIDTH;
		w_copy(con,0, cp_orig);
		con->crtc_start = con->orig;
		con->cursor = con->orig + (SCR_SIZE - SCR_WIDTH) + cursor_x;
		clear_screen(con->cursor, SCR_WIDTH);
		if (!con->is_full)
			con->is_full = 1;
	}

	assert(con->cursor - con->orig < con->con_size);

	while (con->cursor >= con->crtc_start + SCR_SIZE ||
	       con->cursor < con->crtc_start) {
		scroll_screen(con, SCR_UP);

		clear_screen(con->cursor, SCR_WIDTH);
	}

	flush(con);
}


void putfont(int x,int y,char ch)
{
	x=x*FONT_WIDTH;
	y=y*FONT_HEIGHT+TOPBAR_HEIGHT;
	boxfill8(SCREEN_WIDTH,DEFAULT_BACKGROUND,x,y,x+FONT_WIDTH,y+FONT_HEIGHT);
	putfonts8_char( x,y,DEFAULT_FRONT, ch);
}

/*****************************************************************************
 *                                clear_screen
 *****************************************************************************/
/**
 * Write whitespaces to the screen.
 * 
 * @param pos  Write from here.
 * @param len  How many whitespaces will be written.
 *****************************************************************************/
PRIVATE void clear_screen(int pos, int len)
{
	// u8 * pch = (u8*)(V_MEM_BASE + pos * 2);
	// while (--len >= 0) {
	// 	*pch++ = ' ';
	// 	*pch++ = DEFAULT_CHAR_COLOR;
	// }
	while (--len >= 0) {
	int cursor_x = (pos) % SCR_WIDTH;
	int cursor_y = (pos) / SCR_WIDTH;
	putfont(cursor_x,cursor_y,' ');
	pos++;
	}
}


/*****************************************************************************
 *                            is_current_console
 *****************************************************************************/
/**
 * Uses `nr_current_console' to determine if a console is the current one.
 * 
 * @param con   Ptr to console.
 * 
 * @return   TRUE if con is the current console.
 *****************************************************************************/
PUBLIC int is_current_console(CONSOLE* con)
{
	return (con == &console_table[current_console]);
}


/*****************************************************************************
 *                                set_cursor
 *****************************************************************************/
/**
 * Display the cursor by setting CRTC (6845 compatible) registers.
 * 
 * @param position  Position of the cursor based on the beginning of the video
 *                  memory. Note that it counts in WORDs, not in BYTEs.
 *****************************************************************************/
PRIVATE void set_cursor(unsigned int position)
{
	disable_int();
	out_byte(CRTC_ADDR_REG, CURSOR_H);
	out_byte(CRTC_DATA_REG, (position >> 8) & 0xFF);
	out_byte(CRTC_ADDR_REG, CURSOR_L);
	out_byte(CRTC_DATA_REG, position & 0xFF);
	enable_int();
}


/*****************************************************************************
 *                                set_video_start_addr
 *****************************************************************************/
/**
 * Routine for hardware screen scrolling.
 * 
 * @param addr  Offset in the video memory.
 *****************************************************************************/
PRIVATE void set_video_start_addr(u32 addr)
{
	disable_int();
	out_byte(CRTC_ADDR_REG, START_ADDR_H);
	out_byte(CRTC_DATA_REG, (addr >> 8) & 0xFF);
	out_byte(CRTC_ADDR_REG, START_ADDR_L);
	out_byte(CRTC_DATA_REG, addr & 0xFF);
	enable_int();
}


/*****************************************************************************
 *                                select_console
 *****************************************************************************/
/**
 * Select a console as the current.
 * 
 * @param nr_console   Console nr, range in [0, NR_CONSOLES-1].
 *****************************************************************************/
PUBLIC void select_console(int nr_console)
{
	if ((nr_console < 0) || (nr_console >= NR_CONSOLES)) return;
	current_console = 0;
	// flush(&console_table[current_console = nr_console]);
}


/*****************************************************************************
 *                                scroll_screen
 *****************************************************************************/
/**
 * Scroll the screen.
 *
 * Note that scrolling UP means the content of the screen will go upwards, so
 * that the user can see lines below the bottom. Similarly scrolling DOWN means
 * the content of the screen will go downwards so that the user can see lines
 * above the top.
 *
 * When there is no line below the bottom of the screen, scrolling UP takes no
 * effects; when there is no line above the top of the screen, scrolling DOWN
 * takes no effects.
 * 
 * @param con   The console whose screen is to be scrolled.
 * @param dir   SCR_UP : scroll the screen upwards;
 *              SCR_DN : scroll the screen downwards
 *****************************************************************************/
PUBLIC void scroll_screen(CONSOLE* con, int dir)
{
	/*
	 * variables below are all in-console-offsets (based on con->orig)
	 */
	int oldest; /* addr of the oldest available line in the console */
	int newest; /* .... .. ... latest ......... .... .. ... ....... */
	int scr_top;/* position of the top of current screen */

	newest = (con->cursor - con->orig) / SCR_WIDTH * SCR_WIDTH;
	oldest = con->is_full ? (newest + SCR_WIDTH) % con->con_size : 0;
	scr_top = con->crtc_start - con->orig;

	if (dir == SCR_DN) {
		if (!con->is_full && scr_top > 0) {
			con->crtc_start -= SCR_WIDTH;
		}
		else if (con->is_full && scr_top != oldest) {
			if (con->cursor - con->orig >= con->con_size - SCR_SIZE) {
				if (con->crtc_start != con->orig)
					con->crtc_start -= SCR_WIDTH;
			}
			else if (con->crtc_start == con->orig) {
				scr_top = con->con_size - SCR_SIZE;
				con->crtc_start = con->orig + scr_top;
			}
			else {
				con->crtc_start -= SCR_WIDTH;
			}
		}
	}
	else if (dir == SCR_UP) {
		if (!con->is_full && newest >= scr_top + SCR_SIZE) {
			con->crtc_start += SCR_WIDTH;
		}
		else if (con->is_full && scr_top + SCR_SIZE - SCR_WIDTH != newest) {
			if (scr_top + SCR_SIZE == con->con_size)
				con->crtc_start = con->orig;
			else
				con->crtc_start += SCR_WIDTH;
		}
	}
	else {
		assert(dir == SCR_DN || dir == SCR_UP);
	}

	flush(con);
}


/*****************************************************************************
 *                                flush
 *****************************************************************************/
/**
 * Set the cursor and starting address of a console by writing the
 * CRT Controller Registers.
 * 
 * @param con  The console to be set.
 *****************************************************************************/
PRIVATE void flush(CONSOLE* con)
{
	// if (is_current_console(con)) {
	// 	set_cursor(con->cursor);
	// 	set_video_start_addr(con->crtc_start);
	// }

#ifdef __TTY_DEBUG__
	int lineno = 0;
	for (lineno = 0; lineno < con->con_size / SCR_WIDTH; lineno++) {
		u8 * pch = (u8*)(V_MEM_BASE +
				   (con->orig + (lineno + 1) * SCR_WIDTH) * 2
				   - 4);
		*pch++ = lineno / 10 + '0';
		*pch++ = RED_CHAR;
		*pch++ = lineno % 10 + '0';
		*pch++ = RED_CHAR;
	}
#endif
}

/*****************************************************************************
 *                                w_copy
 *****************************************************************************/
/**
 * Copy data in WORDS.
 *
 * Note that the addresses of dst and src are not pointers, but integers, 'coz
 * in most cases we pass integers into it as parameters.
 * 
 * @param dst   Addr of destination.
 * @param src   Addr of source.
 * @param size  How many words will be copied.
 *****************************************************************************/
PRIVATE	void w_copy(CONSOLE *con,unsigned int dst,unsigned int src)
{
	int k,size=SCR_SIZE-SCR_WIDTH;
	char *cbuf=con->cbuf;
	for(k=0;k<size;k++)
	{
		cbuf[dst+k]=cbuf[src+k];
	}
	for(k=size;k<SCR_SIZE;k++)
	{
		cbuf[dst+k]=' ';
	}
	for(k=0;k<SCR_SIZE;k++)
	{
		int x=k%SCR_WIDTH;
		int y=k/SCR_WIDTH;
		putfont(x,y,cbuf[k]);
	}
	
	
}


