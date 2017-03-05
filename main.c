#include <stdio.h>

#include "parse.h"

int main(int argc, char const *argv[])
{
	/*
	initScanner();

	int tok;
	while ( (tok = getToken() ) != ENDFILE ) {
		printToken(tok);
	}
	*/

	parse();

	return 0;
}
