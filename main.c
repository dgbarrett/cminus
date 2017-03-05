#include <stdio.h>

#include "parse.h"
#include "SyntaxTree.h"

int main(int argc, char const *argv[])
{
	/*
	initScanner();

	int tok;
	while ( (tok = getToken() ) != ENDFILE ) {
		printToken(tok);
	}
	*/

	ASTNode * root = parse();
	printSyntaxTree(root);

	return 0;
}
