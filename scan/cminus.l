%{
	#include "scan/tokens.h"
	#include "cminus.tab.h"
	/* any included c code goes here */

	int linenum = 1;
%}

%option noyywrap

if 			"if"
else 		"else"
int   		"int"
void 		"void"
return  	"return"
while   	"while"
plus		"+"
minus		"-"
mul			"*"
div			"/"
le 			"<="
lt			"<"
ge 			">="
gt 			">"
eq			"=="
noteq   	"!="
assignment 	"="
comma 		","
endstmt		";"
lbrace		"("
rbrace		")"
lbracket	"["
rbracket	"]"
lcurl		"{"
rcurl		"}"
opencmt		"/*"
closecmt	"*/"
id			[A-Za-z]+
number 		[0-9]+
whitespace  [ \t]+
newline		[\n]

int_tok     {int}({whitespace}|{newline})+
void_tok 	{void}({whitespace}|{newline})+

%%

{if}			return IF_TOK;
{else}    		return ELSE_TOK;
{int_tok}			return INT_TOK;
{void} 			return VOID_TOK;
{return} 		return RETURN_TOK;
{while}			return WHILE_TOK;
{plus}			return PLUS_TOK;
{minus} 		return MINUS_TOK;
{mul}			return MUL_TOK;
{div} 			return DIV_TOK;
{le} 			return LE_TOK;
{lt}			return LT_TOK;
{ge} 			return GE_TOK;
{gt} 			return GT_TOK;
{eq}			return EQ_TOK;
{noteq}   		return NOTEQ_TOK;
{assignment} 	return ASSIGN_TOK;
{comma}			return COMMA_TOK;
{endstmt}		return ENDSTMT_TOK;
{lbrace}		return LBRACE_TOK;
{rbrace}		return RBRACE_TOK;
{lbracket}		return LBRACKET_TOK;
{rbracket}		return RBRACKET_TOK;
{lcurl}			return LCURL_TOK;
{rcurl}			return RCURL_TOK;
{opencmt}		return OPENCMT_TOK;
{closecmt}		return CLOSECMT_TOK;
{id}			return ID_TOK;
{number}		return NUM_TOK;
{whitespace}    {}
{newline}		{linenum++;}
.				return ERROR_TOK;

%%

void initScanner() {
	yyin = stdin;
	yyout = stdout;
}

Token * getToken() {
	return new_Token(yylex(), yytext, linenum);
}
