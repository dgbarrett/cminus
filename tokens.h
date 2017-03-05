#ifndef __TOKENS_H__
#define __TOKENS_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* token defs from .y file */
#include "y.tab.h"

typedef struct _Token {
	int type;
	char * value;
} Token;

Token * new_Token(int type, char * value);
int Token_getType(Token * tok);
char * Token_getValue(Token * tok);
void printToken(int tok);
void destroy_Token(Token * tok);

#endif