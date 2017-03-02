#include <stdio.h>
#include "lex.h"

int main(int argc, char const *argv[])
{
	initScanner();

	Token * tok;

	while ( Token_getType( (tok = getNextToken()) ) != ENDFILE ) {
		Token_print(tok);
	}

	return 0;
}
