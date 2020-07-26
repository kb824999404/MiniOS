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


static bool if_game_over;
static bool if_exist_game = false;
static bool stop = false;
static bool hit = false;
static const int width = 50;
static const int height = 20;
static int x, y, food_x, food_y, score;
static int tail_x[200], tail_y[200];
static int ntail = 3;

static void menu();
static void setup();
static void draw();
static void input();
static void loop_game();

typedef enum{
	STOP = 0,
	LEFT,
	RIGHT,
	UP,
	DOWN
}Direction;
Direction Dir;

void menu(){
	int a;
	printf("\n\n");
	printf("GAME: tanchishe     SCORE: %05d    \n", score);
	printf("               --------------------------------------------------");
}

void setup(){
	if_game_over = false;
	srand(time());
	Dir = RIGHT;
	x = width / 2;
	y = height / 2;
	tail_x[0] = x , tail_x[1] = x - 1, tail_x[2] = x - 2;
	tail_y[0] = y, tail_y[1] = y, tail_y[2] = y;
	food_x = rand() % width;
	food_y = rand() % height;
	score = 0;
}

void draw(){
	if (stop == true){
		return;
	}
	clear(); 
	printf("\n\n");
	printf(" GAME: tanchishe     SCORE: %05d    \n", score);
	printf("--------------------------------------------------\n");
	int i;
	for (i = 0; i <= width + 1; i++){
		printf("-");
	}
	printf("\n");
	int p, q;
	for (p = 0; p < height + 1; p++){ 
		for (q =-1; q < width + 1; q++){
			if (q == -1 || q == width){
				printf("|");
			}
			if (p == food_y && q == food_x){
				printf("O");
			}
			else{
				int k = 0;
				bool print = false;
				for (k = 0; k < ntail; k++){
					if (tail_x[k] == q && tail_y[k] == p){
						printf("*");
						print = true;
					}
				}
				if (!print){
					printf(" ");
				}
			}
		}
		printf("\n");
	}
	int j;
	for (j = 0; j <= width + 1; j++){
		printf("-");
	}
}

void input()
{
	char keys[40];
	int r =read(0, keys, 40);
	if(strcmp(keys, "a") == 0)
	{
		if (Dir != RIGHT) {
			Dir = LEFT;
			hit = true;
		}
	}
	else if(strcmp(keys, "w") == 0)
	{
		if (Dir != DOWN) {
			Dir = UP;
			hit = true;
		}
	}
	else if(strcmp(keys, "d") == 0)
	{
		if (Dir != LEFT) {
			Dir = RIGHT;
			hit = true;
		}
	}
	else if(strcmp(keys, "s") == 0)
	{
		if (Dir != UP) {
			Dir = DOWN;
			hit = true;
		}
	}
	else if(strcmp(keys, "x") == 0)
	{
		if_game_over = true;
	}
	else if(strcmp(keys, "p") == 0)
	{
		stop = !stop;
	}
}

void logic()
{
	if (stop == true){
		return;
	}
	
	switch (Dir)
	{
	case UP:
		y--;
		break;
	case DOWN:
		y++;
		break;
	case LEFT:
		x--;
		break;
	case RIGHT:
		x++;
		break;
		}
	if ((x < 0 || width < x || y < 0 || height < y) || (tail_x[0] == tail_x[ntail - 1] && tail_y[0] == tail_y[ntail - 1]))
	{
		if_game_over = true;

		clear();
		printf("------------------------------------------------------------------\n");
		printf("|                            GAME OVER!                          |\n");
		printf("------------------------------------------------------------------\n");
	}

	int last_x = tail_x[0];
	int last_y = tail_y[0];
	int last_x2, last_y2;

	tail_x[0] = x;
	tail_y[0] = y;
	int i = 0;

	for (i = 1; i < ntail; i++)
	{
		last_x2 = tail_x[i];
		last_y2 = tail_y[i];
		tail_x[i] = last_x;
		tail_y[i] = last_y;
		last_x = last_x2;
		last_y = last_y2;
	}


	if (x == food_x && y == food_y)
	{
		ntail++;
		score += 10;
		food_x = rand() % width;
		food_y = rand() % height;
	}

}
void loop_game() {
	while (1) {
		if (!if_game_over) {
			draw();
			input();
			logic();
			milli_delay(70);
		}
		else {
			char choice[40];
			int r =read(0, choice, 40);
			choice[r]=0;
			printf("\r\nGAME OVER! TRY THE GAME AGAIN? [Y/N]:     \b\b\b\b");
			if(strcmp(choice, "Y") == 0||strcmp(choice, "y") == 0)
			{
				setup();
			}
			else return;
		}
	}
}

void runtanchishe()
{
#if 0
	while (1)
	{
		printf("%d\n", _getch());
	}
#endif
	menu();
	setup();
	draw();
	loop_game();
}