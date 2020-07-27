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

#define EMPTY_NUM -9999999
#define EMPTY_OP '\0'


char str[400] = "\0";
bool flag = true;

int num_stack[200] = { 0 };
int num_stack_top = -1;
int left = 0, right = 0;

bool num_stack_is_empty(){
	return (num_stack_top == -1) ? true : false;
}

void num_stack_push(int num) {
	num_stack_top++;
	num_stack[num_stack_top] = num;
}

int num_stack_pop() {
	if (num_stack_is_empty()) {
		return EMPTY_NUM;
	}
	int num = num_stack[num_stack_top];
	num_stack[num_stack_top] = 0;
	num_stack_top--;
	return num;
}

void num_stack_clear() {
	int i;
	for (i = 0; i < 200; i++) {
		num_stack[i] = 0;
	}
	num_stack_top = -1;
}

int num_stack_get_top() {
	if (num_stack_is_empty()) {
		return EMPTY_NUM;
	}
	return num_stack[num_stack_top];
}

char op_stack[200] = "\0";
int op_stack_top = -1;

bool op_stack_is_empty() {
	return (op_stack_top == -1) ? true : false;
}

void op_stack_push(char ch) {
	op_stack_top++;
	op_stack[op_stack_top] = ch;
}

char op_stack_pop() {
	if (op_stack_is_empty()) {
		return EMPTY_OP;
	}
	char ch = op_stack[op_stack_top];
	op_stack[op_stack_top] = '\0';
	op_stack_top--;
	return ch;
}

char op_stack_get_top() {
	if (op_stack_is_empty()) {
		return EMPTY_OP;
	}
	return op_stack[op_stack_top];
}

void op_stack_clear() {
	int i;
	for (i = 0; i < 200; i++) {
		op_stack[i] = '\0';
	}
	op_stack_top = -1;
}

bool is_digit(char ch)
{
	if (ch >= '0'&&ch <= '9')
		return true;
	else return false;
}

int isp(char ch){
	if (ch == '#' || ch == '=')return 0;
	else if (ch == '(')return 1;
	else if (ch == '+' || ch == '-')return 3;
	else if (ch == '*' || ch == '/')return 5;
	else if (ch == '%')return 7;
	else if (ch == '^')return 9;
	else if (ch == ')')return 10;
	else{
		printf("The arithmetic expression you entered contains illegal characters.Please enter again.\n");
		flag = false;
		return -1;
	}
}

int icp(char ch){
	if (ch == '#' || ch == '=')return 0;
	else if (ch == '(')return 10;
	else if (ch == '+' || ch == '-')return 2;
	else if (ch == '*' || ch == '/')return 4;
	else if (ch == '%')return 6;
	else if (ch == '^')return 8;
	else if (ch == ')')return 1;
	else
	{
		printf("The arithmetic expression you entered contains illegal characters.Please enter again.\n");
		flag = false;
		return -1;
	}
}
int pow(int a,int b)
{
	int i,y=1;
	for(i=0;i<a;i++)
	{
		y*=b;
	}
	return y;
}

bool get_two_num() {
	if (num_stack_is_empty()) {
		printf("The arithmetic expression you entered lacks left numbers.Please enter again.\n");
		flag = false;
		return false;
	}
	left = num_stack_pop();
	if (num_stack_is_empty()) {
		printf("The arithmetic expression you entered lacks right numbers.Please enter again.\n");
		flag = false;
		return false;
	}
	right = num_stack_pop();
	return true;
}

void do_operator(char ch) {
	int value = 0;
	bool result = get_two_num();
	if (result) {
		switch (ch)
		{
		case '+':value = left + right; num_stack_push(value); break;
		case '-':value = left - right; num_stack_push(value); break;
		case '*':value = left * right; num_stack_push(value); break;
		case '%':
		{
			int temp1 = left, temp2 = right;
			value = temp1 % temp2;
			num_stack_push(value);
			break;
		}
		case '^':value = pow(left, right); num_stack_push(value); break;
		case '/':
		{
			if (right == 0.0)
			{
				printf("You can't divide by zero.Please enter again.\n");
				flag = false;
			}
			else
			{
				value = left / right;
				num_stack_push(value);
				break;
			}
		}
		}
	}
}

void run() {
	char ch = '#', ch1, op;
	int number, length = strlen(str);
	if (str[length - 1] != '=') {
		str[length] = '=';
		length++;
	}
	op_stack_push(ch);
	int i;
	for (i = 0; i < length;) {
		if (is_digit(str[i])) {
			char temp[400] = "\0";
			int j = 0;
			while (is_digit(str[i])) {
				temp[j] = str[i];
				i++, j++;
			}
			number = atoi(temp);
			//printf(" %d", number);
			num_stack_push(number);
		}
		else {
			ch1 = op_stack_get_top();
			//printf(" %c", ch1);
			if (isp(ch1) < icp(str[i])) {
				op_stack_push(str[i]);
				i++;
			}
			else if (isp(ch1) > icp(str[i])) {
				op = op_stack_pop();
				do_operator(op);
			}
			else {
				op = op_stack_pop();
				if (op == '(' || op == '#')i++;
			}
		}
	}
}

bool format() {
	int length = strlen(str);
	int i;
	for (i = 0; i < length; i++){
		if (i == 0){
			if (!is_digit(str[i]) && str[i] != '('){
				flag = false;
				return false;
			}
		}
		else if (i == length - 1){
			if (!is_digit(str[i]) && str[i] != ')' && str[i] != '='){
				flag = false;
				return false;
			}
		}
		else{
			if (!is_digit(str[i]) && (str[i] != '(' && str[i] != ')')){
				if ((!is_digit(str[i + 1]) && str[i + 1] != '(') || (!is_digit(str[i - 1]) && str[i - 1] != ')')){
					flag = false;
					return false;
				}
			}
			if (str[i] == '(' && (is_digit(str[i - 1]) || !is_digit(str[i + 1]))){
				flag = false;
				return false;
			}
			if (str[i] == ')' && (!is_digit(str[i - 1]) || is_digit(str[i + 1]))){
				flag = false;
				return false;
			}
		}
	}
	return true;
}

void runcalculator() {
	printf("\nPlease input an arithmetic expression:\n");
	int r = read(0, str, 400);
	str[r] = 0;
	if (!format()) {
		printf("\nThe arithmetic expression you input is not correct.Please enter again.\n");
		return;
	}
	else{
		run();
		if (flag) {
			int number = num_stack_get_top();
			printf("\nThe result is %d.\n", number);
		}
	}

}
