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
#include "graphics.h"

static bool if_game_over;
static bool if_exist_game = false;
static bool stop = false;
static const int width = 50;
static const int height = 20;
static int x, y, food_x, food_y, score;
static int tail_x[200], tail_y[200];
static int ntail = 3;
static const int drawSize=5;
static const int centerX=SCREEN_WIDTH/2;
static const int centerY=PANELSIZE/2+TOPBAR_HEIGHT;

static void menu();
static void setup();
static void draw();
static void input();
static void loop_game();

typedef enum{
	STOP = 0,
	DIR_LEFT,
	DIR_RIGHT,
	DIR_UP,
	DIR_DOWN
}Direction;
Direction Dir;

void menu(){
	char s[40];
	clearAll();
	sprintf(s,"GAME: tanchishe   SCORE: %05d",score);
	drawFont(FONT_WIDTH,FONT_HEIGHT/2,s,COLOR_WHITE);
}

void setup(){
	if_game_over = false;
	srand(time());
	Dir = DIR_RIGHT;
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
	char s[40];
	clearAll();
	sprintf(s,"GAME: tanchishe   SCORE: %05d",score);
	drawFont(FONT_WIDTH,FONT_HEIGHT/2,s,COLOR_WHITE);
	int i;
	int p, q;
	int X0=centerX-(width+2)*drawSize/2;
	int Y0=centerY-(height+2)*drawSize/2;
	drawBox(X0,Y0,X0+(width+2)*drawSize,Y0+(height+2)*drawSize,COLOR_WHITE);
	for (p = 0; p < height + 1; p++){ 
		for (q =-1; q < width + 1; q++){
			if (p == food_y && q == food_x){
				drawBox(X0+(q+1)*drawSize,Y0+(p+1)*drawSize,X0+(q+2)*drawSize,Y0+(p+2)*drawSize,COLOR_RED);
			}
			else{
				int k = 0;
				for (k = 0; k < ntail; k++){
					if (tail_x[k] == q && tail_y[k] == p){
						drawBox(X0+(q+1)*drawSize,Y0+(p+1)*drawSize,X0+(q+2)*drawSize,Y0+(p+2)*drawSize,COLOR_BRIGHTYELLOW);
					}
				}
			}
		}
	}
	drawFont(FONT_WIDTH,SCREEN_HEIGHT-FONT_HEIGHT*2,"WSAD:Move  P:Pause  Enter:Exit",COLOR_WHITE);
}

void input()
{
	u32 key=kbhit();
	if (!(key & FLAG_EXT)) {
		switch (key)
		{
			case 'a':
			case 'A':
				if (Dir != DIR_RIGHT) {
					Dir = DIR_LEFT;
				}
				break;
			case 'w':
			case 'W':
				if (Dir != DIR_DOWN) {
					Dir = DIR_UP;
				}
				break;
			case 'd':
			case 'D':
				if (Dir != DIR_LEFT) {
					Dir = DIR_RIGHT;
				}
				break;
			case 's':
			case 'S':
				if (Dir != DIR_UP) {
					Dir = DIR_DOWN;
				}
				break;
			case 'p':
			case 'P':
				stop = !stop;
				if(stop)
				{
					drawFont(FONT_WIDTH,FONT_HEIGHT*2,"PAUSE",COLOR_RED);
				}
				break;
		}
	}
	else
	{
		int raw_code = key & MASK_RAW;
		switch(raw_code) {
			case LEFT:
				if (Dir != DIR_RIGHT) {
					Dir = DIR_LEFT;
				}
				break;
			case UP:
				if (Dir != DIR_DOWN) {
					Dir = DIR_UP;
				}
				break;
			case RIGHT:
				if (Dir != DIR_LEFT) {
					Dir = DIR_RIGHT;
				}
				break;
			case DOWN:
				if (Dir != DIR_UP) {
					Dir = DIR_DOWN;
				}
				break;
			case ENTER:
				if_game_over = true;
				break;
		}
	}
	

}

void logic()
{
	if (stop == true){
		return;
	}
	
	switch (Dir)
	{
	case DIR_UP:
		y--;
		break;
	case DIR_DOWN:
		y++;
		break;
	case DIR_LEFT:
		x--;
		break;
	case DIR_RIGHT:
		x++;
		break;
		}
	if ((x < 0 || width < x || y < 0 || height < y) || (tail_x[0] == tail_x[ntail - 1] && tail_y[0] == tail_y[ntail - 1]))
	{
		if_game_over = true;
		return;
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
		tail_x[ntail-1] = tail_x[ntail-2];
		tail_y[ntail-1] = tail_y[ntail-2];
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
			milli_delay(1000);
		}
		else {
			clearAll();
			char *s="GAME OVER!";
			drawFont(centerX-strlen(s)/2*FONT_WIDTH,centerY,s,COLOR_RED);
			s="TRY THE GAME AGAIN? [Y/N]";
			drawFont(centerX-strlen(s)/2*FONT_WIDTH,centerY+FONT_HEIGHT,s,COLOR_BRIGHTYELLOW);
			u32 key=kbhit();
			if(key=='y'||key=='Y')
			{
				setup();

			}
			else return;
		}
	}
}

void runtanchishe()
{
	menu();
	setup();
	draw();
	loop_game();
	clearAll();
}