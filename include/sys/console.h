
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			      console.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
						    Forrest Yu, 2005
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#ifndef _ORANGES_CONSOLE_H_
#define _ORANGES_CONSOLE_H_



#define SCR_UP	1	/* scroll upward */
#define SCR_DN	-1	/* scroll downward */

#define SCR_SIZE		(40 * 11)
#define SCR_WIDTH		 40
#define TAB_WIDTH		 8
#define PIX_WIDTH		 320
#define PIX_HEIGHT		 200

/* CONSOLE */
typedef struct s_console
{
	unsigned int	crtc_start; /* set CRTC start addr reg */
	unsigned int	orig;	    /* start addr of the console */
	unsigned int	con_size;   /* how many words does the console have */
	unsigned int	cursor;
	int		is_full;
	char cbuf[SCR_SIZE];
}CONSOLE;

// #define DEFAULT_CHAR_COLOR	(MAKE_COLOR(BLACK, WHITE))
#define DEFAULT_CHAR_COLOR  (MAKE_COLOR(BLACK, GREEN|BLUE) | BRIGHT)
#define GRAY_CHAR		(MAKE_COLOR(BLACK, BLACK) | BRIGHT)
#define RED_CHAR		(MAKE_COLOR(BLUE, RED) | BRIGHT)

#endif /* _ORANGES_CONSOLE_H_ */
