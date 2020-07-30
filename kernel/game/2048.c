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
#include "keyboard.h"


void init_game();
void loop_game();
void reset_game();
void release_game();

void move_left();
void move_right();
void move_up();
void move_down();
void add_rand_num();
void check_game_over();
int get_null_count();
void refresh_screen();

static int board[4][4];
static int score;
static int best_score;
static int if_need_add_num;
static int if_game_over;
static int if_prepare_exit;
static const int centerX=SCREEN_WIDTH/2;
static const int centerY=PANELSIZE/2+TOPBAR_HEIGHT;
static int gridSizeX=2*FONT_HEIGHT-2;
static int gridSizeY=2*FONT_HEIGHT-2;

void run2048()
{
	init_game();
	loop_game();
}

void loop_game() {
	while (1) {
		u32 key=kbhit();
		refresh_screen();


		if (if_prepare_exit) {
			if (key == 'y' || key == 'Y') {
				clearAll();
				return;
			}
			else if (key == 'n' || key == 'N') {
				if_prepare_exit = 0;
				refresh_screen();
				continue;
			}
			else {
				continue;
			}
		}

		if (if_game_over) {
			if (key == 'y' || key == 'Y') {
				reset_game();
				continue;
			}
			else if (key == 'n' || key == 'N') {
				clearAll();
				return;
			}
			else {
				continue;
			}
		}

		if_need_add_num = 0;
		switch (key)
		{
		case 'w':
			move_up();
			break;
		case 'a':
			move_left();
			break;
		case 's':
			move_down();
			break;
		case 'd':
			move_right();
			break;
		case 'q':
			if_prepare_exit = 1;
			break;
		default:
			refresh_screen();
			break;
		}

		if (if_need_add_num) {
			add_rand_num();
			refresh_screen();
		}
		else if (if_prepare_exit) {
			refresh_screen();
		}
	}
}

void reset_game() {
	score = 0;
	if_need_add_num = 1;
	if_game_over = 0;
	if_prepare_exit = 0;

	srand(time());
	int x = rand() % 4;
	int y = rand() % 4;
	int i, j;
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			board[i][j] = 0;
		}
	}
	board[x][y] = 2;

	add_rand_num();
	refresh_screen();
}

void add_rand_num() {
	srand(time());
	int n = rand() % get_null_count();
	int i, j;
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			if (board[i][j] == 0 && n-- == 0) {
				board[i][j] = (rand() % 5 ? 2 : 4);
				return;
			}
		}
	}
}

int get_null_count() {
	int num = 0;
	int i, j;
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			if (board[i][j] == 0)
				num++;
		}
	}
	return num;
}

void check_game_over() {
	int i, j;
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 3; j++) {
			if (board[i][j] == board[i][j + 1] || board[j][i] == board[j + 1][i]) {
				if_game_over = 0;
				return;
			}
		}
	}
	if_game_over = 1;
}


void move_left() {
	int i, j, k;
	for (i = 0; i < 4; i++) {
		for (j = 1, k = 0; j < 4; j++) {
			if (board[i][j] > 0) {
				if (board[i][k] == board[i][j]) {
					board[i][k] = board[i][k] * 2;
					score += board[i][k];
					k++;
					board[i][j] = 0;
					if_need_add_num = 1;
				}
				else if (board[i][k] == 0) {
					board[i][k] = board[i][j];
					board[i][j] = 0;
					if_need_add_num = 1;
				}
				else {
					k++;
					board[i][k] = board[i][j];
					if (j != k) {
						board[i][j] = 0;
						if_need_add_num = 1;
					}
				}
			}
		}
	}
}

void move_right() {
	int i, j, k;
	for (i = 0; i < 4; i++) {
		for (j = 2, k = 3; j >= 0; j--) {
			if (board[i][j] > 0) {
				if (board[i][k] == board[i][j]) {
					board[i][k] = board[i][k] * 2;
					score += board[i][k];
					k--;
					board[i][j] = 0;
					if_need_add_num = 1;

				}

				else if (board[i][k] == 0) {
					board[i][k] = board[i][j];
					board[i][j] = 0;
					if_need_add_num = 1;
				}

				else {
					k--;
					board[i][k] = board[i][j];
					if (j != k)  {
						board[i][j] = 0;
						if_need_add_num = 1;
					}
				}
			}
		}
	}
}

void move_up() {
	int i, j, k;
	for (i = 0; i < 4; i++) {
		for (j = 1, k = 0; j < 4; j++) {
			if (board[j][i] > 0) {
				if (board[k][i] == board[j][i]) {
					board[k][i] = board[k][i] * 2;
					score = board[k][i];
					k++;
					board[j][i] = 0;
					if_need_add_num = 1;
				}

				else if (board[k][i] == 0) {
					board[k][i] = board[j][i];
					board[j][i] = 0;
					if_need_add_num = 1;
				}
				else {
					k++;
					board[k][i] = board[j][i];
					if (j != k) {
						board[j][i] = 0;
						if_need_add_num = 1;
					}
				}
			}
		}
	}
}

void move_down() {
	int i, j, k;
	for (i = 0; i < 4; i++) {
		for (j = 2, k = 3; j >= 0; j--) {
			if (board[j][i] > 0) {
				if (board[k][i] == board[j][i]) {
					board[k][i] = board[k][i] * 2;
					score += board[k][i];
					k--;
					board[j][i] = 0;
					if_need_add_num = 1;
				}
				else if (board[k][i] == 0) {
					board[k][i] = board[j][i];
					board[j][i] = 0;
					if_need_add_num = 1;
				}
				else {
					k--;
					board[k][i] = board[j][i];
					if (j != k) {
						board[j][i] = 0;
						if_need_add_num = 1;
					}
				}
			}
		}
	}
}

void refresh_screen() {
	clearAll();
	char s[40];
	sprintf(s,"GAME: 2048   SCORE: %05d",score);
	drawFont(FONT_WIDTH,FONT_HEIGHT/2,s,COLOR_WHITE);
	drawBox(centerX-2*gridSizeX,centerY-2*gridSizeY,centerX+2*gridSizeX,centerY+2*gridSizeY,COLOR_WHITE);
	int i, j;
	for(i=0;i<=4;i++)
	{
		drawLine(centerX-2*gridSizeX+i*gridSizeX,centerY-2*gridSizeY,centerX-2*gridSizeX+i*gridSizeX,centerY+2*gridSizeY,COLOR_BLACK);
		drawLine(centerX-2*gridSizeX,centerY-2*gridSizeY+i*gridSizeY,centerX+2*gridSizeX,centerY-2*gridSizeY+i*gridSizeY,COLOR_BLACK);
	}
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			if (board[i][j] != 0) {
				if (board[i][j] < 10) {
					sprintf(s,"%d",board[i][j]);
				}
				else if (board[i][j] < 100) {
					sprintf(s,"%d",board[i][j]);
				}
				else if (board[i][j] < 1000) {
					sprintf(s,"%d",board[i][j]);
				}
				else if (board[i][j] < 10000) {
					sprintf(s,"%d",board[i][j]);
				}
				else {
					int n = board[i][j];
					int k;
					for (k = 1; k < 20; k++) {
						n = n >> 1;
						if (n == 1) {
							sprintf(s,"2^%02d",k);
							// printf("2^%02d|", k);
							break;
						}
					}
				}
				int x0=centerX-2*gridSizeX+j*gridSizeX+gridSizeX/2-strlen(s)/2*FONT_WIDTH;
				int y0=centerY-2*gridSizeY+i*gridSizeY+gridSizeY/2-FONT_HEIGHT/2;
				drawFont(x0,y0,s,COLOR_BLACK);
			}
		}
	}
	drawFont(FONT_WIDTH,SCREEN_HEIGHT-FONT_HEIGHT*2,"[WASD]:MOVE  [Q]:EXIT",COLOR_WHITE);
	
	if (get_null_count() == 0) {
		check_game_over();
		if (if_game_over) {
			clearAll();
			char *s="GAME OVER!";
			drawFont(centerX-strlen(s)/2*FONT_WIDTH,centerY,s,COLOR_RED);
			s="TRY THE GAME AGAIN? [Y/N]";
			drawFont(centerX-strlen(s)/2*FONT_WIDTH,centerY+FONT_HEIGHT,s,COLOR_BRIGHTYELLOW);
		}
	}
	if (if_prepare_exit) {
		clearAll();
		char *s="Quit Game? [Y/N]";
		drawFont(centerX-strlen(s)/2*FONT_WIDTH,centerY,s,COLOR_RED);
	}
}

void init_game() {
	reset_game();
}

void release_game() {
	clearAll();
}