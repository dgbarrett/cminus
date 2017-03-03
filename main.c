#include <stdio.h>

#include "lex.h"
#include "parse.h"

#define DEBUG

int main(int argc, char const *argv[])
{
	/*
	initScanner();

	int tok;
	while ( (tok = getToken() ) != ENDFILE ) {
		printToken(tok);
	}
	*/

	initParser();
	parse();

	return 0;
}
