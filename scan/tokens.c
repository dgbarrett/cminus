#include "tokens.h"
#include "../cminus.tab.h"

Token * new_Token(int type, char * value, int linenum) {
	Token * tok = malloc(sizeof(*tok));

	tok -> type = type;
	tok -> linenum = linenum;
	tok -> value = calloc(strlen(value) + 1, sizeof(*(tok->value)));
	strcpy(tok->value, value);

	return tok;
}

int Token_getType(Token * tok) {
	if (tok) return tok -> type;
	else return -1;
}

char * Token_getValue(Token * tok) {
	if (tok) return tok -> value;
	else return NULL;
}

void destroy_Token(Token * tok) {
	if (tok) {
		if (tok -> value) {
			free(tok->value);
			tok->value = NULL;
		}
		free(tok);
		tok = NULL;
	}
}

#define YY_EOF 0

void printToken(int tok) {
	switch(tok) {
		case IF_TOK:
			printf("IF_TOK\n");
			break;
		case ELSE_TOK:
			printf("ELSE_TOK\n");
			break;
		case INT_TOK:
			printf("INT_TOK\n");
			break;
		case VOID_TOK:
			printf("VOID_TOK\n");
			break;
		case WHILE_TOK:
			printf("WHILE_TOK\n");
			break;
		case RETURN_TOK:
			printf("RETURN_TOK\n");
			break;
		case PLUS_TOK:
			printf("PLUS_TOK\n");
			break;
		case MINUS_TOK:
			printf("MINUS_TOK\n");
			break;
		case MUL_TOK:
			printf("MUL_TOK\n");
			break;
		case DIV_TOK:
			printf("DIV_TOK\n");
			break;
		case LE_TOK:
			printf("LE_TOK\n");
			break;
		case LT_TOK:
			printf("LT_TOK\n");
			break;
		case GE_TOK:
			printf("GE_TOK\n");
			break;
		case GT_TOK:
			printf("GT_TOK\n");
			break;
		case EQ_TOK:
			printf("EQ_TOK\n");
			break;
		case NOTEQ_TOK:
			printf("NOTEQ_TOK\n");
			break;
		case ASSIGN_TOK:
			printf("ASSIGN_TOK\n");
			break;
		case COMMA_TOK:
			printf("COMMA_TOK\n");
			break;
		case ENDSTMT_TOK:
			printf("ENDSTMT_TOK\n");
			break;
		case LBRACE_TOK:
			printf("LBRACE_TOK\n");
			break;
		case RBRACE_TOK:
			printf("RBRACE_TOK\n");
			break;
		case LBRACKET_TOK:
			printf("LBRACKET_TOK\n");
			break;
		case RBRACKET_TOK:
			printf("RBRACKET_TOK\n");
			break;
		case LCURL_TOK:
			printf("LCURL_TOK\n");
			break;
		case RCURL_TOK:
			printf("RCURL_TOK\n");
			break;
		case OPENCMT_TOK:
			printf("OPENCMT_TOK\n");
			break;
		case CLOSECMT_TOK:
			printf("CLOSECMT_TOK\n");
			break;
		case ID_TOK:
			printf("ID_TOK\n");
			break;
		case NUM_TOK:
			printf("NUMBER_TOK\n");
			break;
		case WHITESPACE_TOK:
			printf("WHITESPACE_TOK\n");
			break;
		case ERROR_TOK:
			printf("ERROR_TOK\n");
			break;
		case YY_EOF:
			printf("EOF\n");
			break;
		default:
			printf("INVALID %d\n", EOF);
			break;
	}
}
