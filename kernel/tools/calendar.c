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

bool is_leap(int year);
int day_count(int year, int month);
int total_day(int year, int month, int day);
int weekday(int year, int month, int day);
void print_weekday(int year, int month, int day);
void print_month(int year, int month);
void print_total_day(int year, int month, int day);



void runcalendar() {
	int year, month, day;
	printf("Calendar\n");
	printf("year:");
	year=readint();
	printf("month:");
	month=readint();
	printf("day:");
	day=readint();
	// printf("%d\t%d\t%d\n",year,month,day);
	print_weekday(year, month, day);
	
}

bool is_leap(int year) {
	if (year % 400 == 0 || (year % 4 == 0 && year % 100 != 0))
		return true;
	else
		return false;
}

int day_count(int year, int month) {
	int day[12] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
	if (is_leap) {
		day[1] = 29;
	}
	return day[month - 1];
}

int total_day(int year,int month,int day) {
	int sum = 0;
	int i;
	for (i = 1; i < month; i++)
		sum = sum + day_count(year, i);
	sum = sum + day;
	return sum;
}

int weekday(int year, int month, int day){
	int count;
	count = (year - 1) + (year - 1) / 4 - (year - 1) / 100 + (year - 1) / 400 + total_day(year, month, day);
	count = count % 7;
	return count;
}

void print_weekday(int year, int month, int day) {
	if (year < 0) {
		printf("The [year] you input should be more than 0.Please input again.\n");
		return;
	}
	if (month > 12 || month < 0) {
		printf("The [month] you input should be between 0 and 12.Please input again.\n");
		return;
	}
	if (day<0 || day>day_count(year, month)) {
		printf("The [day] you input should be between 0 and %d.Please input again.\n", day_count(year, month));
		return;
	}
	int count;
	count = weekday(year, month, day);
	switch (count)
	{
	case 0:
		printf("\n%4d-%d-%d is Sunday.\n", year, month, day); 
		break;
	case 1:
		printf("\n%d-%d-%d is Monday.\n", year, month, day); 
		break;
	case 2:
		printf("\n%d-%d-%d is Tuesday.\n", year, month, day); 
		break;
	case 3:
		printf("\n%d-%d-%d is Wednesday.\n", year, month, day); 
		break;
	case 4:
		printf("\n%d-%d-%d is Thursday.\n", year, month, day); 
		break;
	case 5:
		printf("\n%d-%d-%d is Friday.\n", year, month, day); 
		break;
	case 6:
		printf("\n%d-%d-%d is Saturday.\n", year, month, day); 
		break;
	}
}

void print_month(int year,int month) {
	if (year < 0) {
		printf("The [year] you input should be more than 0.Please input again.\n");
		return;
	}
	if (month > 12 || month < 0) {
		printf("The [month] you input should be between 0 and 12.Please input again.\n");
		return;
	}
	int day_num = day_count(year, month);
	int start_day = weekday(year, month, 1);

	printf("\nSun   Mon   Tue   Wed   Thu   Fri   Sat\n");

	int i, j = 0, k = start_day, day = 1;
	for (i = 0; i < start_day; i++) {
		printf("      ");
	}
	while (j < day_num) {
		if (day < 10) {
			printf("%d     ", day);
		}
		else if (day >= 10) {
			printf("%d    ", day);
		}
		day++, k++;
		if (k % 7 == 0) {
			printf("\n");
			k = 0;
		}
		j++;
	}
}

void print_total_day(int year, int month, int day) {
	if (year < 0) {
		printf("The [year] you input should be more than 0.Please input again.\n");
		return;
	}
	if (month > 12 || month < 0) {
		printf("The [month] you input should be between 0 and 12.Please input again.\n");
		return;
	}
	if (day<0 || day>day_count(year, month)) {
		printf("The [day] you input should be between 0 and %d.Please input again.\n", day_count(year, month));
		return;
	}

	printf("\n%d-%d-%d is day No.%d of the year.", year, month, day, total_day(year, month, day));
}