#ifndef __TOKENS_H__
#define __TOKENS_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum _CMinusTokType {
	IF_TOK,
	ELSE_TOK,
	INT_TOK,
	VOID_TOK,
	WHILE_TOK,
	RETURN_TOK,
	PLUS_TOK,
	MINUS_TOK,
	MUL_TOK,
	DIV_TOK,
	LE_TOK,
	LT_TOK,
	GE_TOK,
	GT_TOK,
	EQ_TOK,
	NOTEQ_TOK,
	ASSIGN_TOK,
	COMMA_TOK,
	ENDSTMT_TOK,	
	LBRACE_TOK,	
	RBRACE_TOK,
	LBRACKET_TOK,
	RBRACKET_TOK,	
	LCURL_TOK,	
	RCURL_TOK,
	OPENCMT_TOK,
	CLOSECMT_TOK,
	ID_TOK,
	NUMBER_TOK,
	WHITESPACE_TOK,
	ERROR_TOK,
	ENDFILE
} CMinusTokType;

typedef struct _Token {
	char * val;
	CMinusTokType type;
} Token;

void CMinusTokType_print(CMinusTokType tok);
Token * new_Token(CMinusTokType toktype, char * tokval) ;
CMinusTokType Token_getType(Token * tok);
void Token_print(Token * tok);

#endif