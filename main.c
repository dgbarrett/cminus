#include <stdio.h>
#include "lex.h"

#define DEBUG

int main(int argc, char const *argv[])
{
	initScanner();

	Token * tok;

	while ( Token_getType( (tok = getNextToken()) ) != ENDFILE ) {
		Token_print(tok);
	}

	initParser();
	parse();

	return 0;
}
