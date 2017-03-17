#include <stdio.h>

#include "parse/parse.h"
#include "parse/ast.h"

#define MAX_ARGS 3

int AST_FLAG = 0;
int SYMBTABLE_FLAG = 0;

/* Checks if a given filename represents a c- file */
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

/* Checks if a given string represent a commmand line flag */
int isFlag(const char * flag) {
	return strcmp(flag, "-a") == 0 ||
		   strcmp(flag, "-s") == 0;
}

/* Sets a flag */
void setFlag(const char * flag) {
	if (strcmp(flag, "-a") == 0) {
		AST_FLAG = 1;
	} else if (strcmp(flag, "-s") == 0) {
		SYMBTABLE_FLAG = 1;
	}
}

int main(int argc, char const *argv[])
{
	if (argc <= MAX_ARGS && argc >= 2) {
		const char * sourceFile = NULL;
		int i;

		/* get input file */
		for (i = 1 ; i < argc ; i++) {
			if (isCMinusFile(argv[i])){
				sourceFile = argv[i];
				break;
			}
		}

		/* get and set flags */
		for (i = 1 ; i < argc ; i++) {
			if (isFlag(argv[i])) {
				setFlag(argv[i]);
			}
		}

		FILE * input = fopen(sourceFile, "r");

		if (input) {
			/* parse out ast */
			ASTNode * root = parse(input);

			/* print if requested */
			if (root) {
				if (AST_FLAG) {
					printf("\n");
					printSyntaxTree(root, NIL, 0, 0);
					printf("\n");
				}

				if (SYMBTABLE_FLAG) {
					printf("\n");
					printf("[DUMMY SYMBOL TABLE PRINT]\n");
					/* printSymbolTable(root) */
					printf("\n");
				}
			}
			
		} else{
			fprintf(stderr, "Invalid file name. Exiting.\n");
			exit(0);
		}
	} else {
		fprintf(stderr, "Too many arguments provided.  Expected max of %d, found %d\n", MAX_ARGS, argc);
	}

	return 0;
}
