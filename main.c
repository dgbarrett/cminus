#include <stdio.h>

#include "parse/parse.h"
#include "parse/ast.h"

#define MAX_ARGS 3

int AST_FLAG = 0;

int isCMinusFile(const char * fname) {
	if (fname) {
		int len = strlen(fname) + 1, i;
		if (len > 4) {
			for (i=len ; i >= 0 ; i--) {
				if (fname[i] == '.') {
					return fname[i+1] == 'c' && fname[i+2] == 'm';
				}
			}
		} 
	}
	return 0;
}

int isFlag(const char * flag) {
	return strcmp(flag, "-a") == 0;
}

void setFlag(const char * flag) {
	AST_FLAG = 1;
}

int main(int argc, char const *argv[])
{
	/*
	initScanner();

	int tok;
	while ( (tok = getToken() ) != ENDFILE ) {
		printToken(tok);
	}
	*/

	if (argc <= MAX_ARGS && argc >= 2) {
		const char * sourceFile = NULL;
		int i;

		for (i = 1 ; i < argc ; i++) {
			if (isCMinusFile(argv[i])){
				sourceFile = argv[i];
				break;
			}
		}

		for (i = 1 ; i < argc ; i++) {
			if (isFlag(argv[i])) {
				setFlag(argv[i]);
				break;
			}
		}

		FILE * input = fopen(sourceFile, "r");
		ASTNode * root = parse(input);

		if (AST_FLAG && root) {
			printf("\n");
			printSyntaxTree(root, NIL, 0, 0);
			printf("\n");
		}
	} else {
		fprintf(stdout, "Too many arguments provided.  Expected max of %d, found %d\n", MAX_ARGS, argc);
	}

	return 0;
}
