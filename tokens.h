#ifndef __TOKENS_H__
#define __TOKENS_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _CMinusToken {
	int type, linenum;
	char * value;
} Token;

typedef enum _CMinusOperator {
	GT,
	GE,
	LT,
	LE,
	EQ,
	NEQ,
	ASSIGN,
	ADD,
	SUB,
	DIV,
	MUL
} Operator;

Token * new_Token(int type, char * value, int linenum);
int Token_getType(Token * tok);
char * Token_getValue(Token * tok);
void printToken(int tok);
void destroy_Token(Token * tok);

#endif