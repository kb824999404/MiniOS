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



void init_game();
void loop_game(int fd_stdin);
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

void run2048()
{
	init_game();
	
}

void loop_game(int fd_stdin) {
	while (1) {
		char buf[128];
		int r = 0;
		r = read(fd_stdin, buf, 70);
		if (r > 1) {
			refresh_screen();
			continue;
		}
		buf[r] = 0;
		char temp = buf[0];

		if (if_prepare_exit) {
			if (temp == 'y' || temp == 'Y') {
				clear();
				return;
			}
			else if (temp == 'n' || temp == 'N') {
				if_prepare_exit = 0;
				refresh_screen();
				continue;
			}
			else {
				continue;
			}
		}

		if (if_game_over) {
			if (temp == 'y' || temp == 'Y') {
				reset_game();
				continue;
			}
			else if (temp == 'n' || temp == 'N') {
				clear();
				return;
			}
			else {
				continue;
			}
		}

		if_need_add_num = 0;
		switch (temp)
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
			move_left();
			break;
		case 'q':
			if_prepare_exit = 1;
			break;
		;default:
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
	int i, j, k = 0;
	for (i = 0; i < 4; i++) {
		for (j = 1; j < 4; j++) {
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
	int i, j, k = 3;
	for (i = 0; i < 4; i++) {
		for (j = 2; j >= 0; j--) {
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
	int i, j, k = 0;
	for (i = 0; i < 4; i++) {
		for (j = 1; j < 4; j++) {
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
	int i, j, k = 3;
	for (i = 0; i < 4; i++) {
		for (j = 2; j >= 0; j--) {
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
	clear();

	printf("\n\n");
	printf("GAME: 2048     SCORE: %05d    \n", score);
	printf("--------------------------------------------------");
	printf("\n\n|----|----|----|----|\n");
	int i, j;
	for (i = 0; i < 4; i++) {
		printf("                             |");
		for (j = 0; j < 4; j++) {
			if (board[i][j] != 0) {
				if (board[i][j] < 10) {
					printf("  %d |", board[i][j]);
				}
				else if (board[i][j] < 100) {
					printf(" %d |", board[i][j]);
				}
				else if (board[i][j] < 1000) {
					printf(" %d|", board[i][j]);
				}
				else if (board[i][j] < 10000) {
					printf("%4d|", board[i][j]);
				}
				else {
					int n = board[i][j];
					int k;
					for (k = 1; k < 20; k++) {
						n = n >> 1;
						if (n == 1) {
							printf("2^%02d|", k);
							break;
						}
					}
				}
			}
			else printf("    |");
		}
		if (i < 3) {
			printf("\n|----|----|----|----|\n");
		}
		else {
			printf("\n|----|----|----|----|\n");
		}
	}
	printf("\n");
	printf("--------------------------------------------------\n");
	printf("[W]:UP [S]:DOWN [A]:LEFT [D]:RIGHT [Q]:EXIT\n");
	printf("Enter your operation here:");

	if (get_null_count() == 0) {
		check_game_over();
		if (if_game_over) {
			printf("\r\nGAME OVER! Try this game again? [Y/N]:     \b\b\b\b");
		}
	}
	if (if_prepare_exit) {
		printf("\r\nDo you really want to quit this game? [Y/N]:   \b\b");
	}
}

void init_game() {
	reset_game();
}

void release_game() {
	clear();
	printf("\n");
}