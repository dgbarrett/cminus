#include "symbtable_print.h"

#include <stdio.h>

void printScope(Scope * scope, int depth);
void printScopeType(ScopeType type, int depth);
void printSymbols(Symbol ** symbols, int depth);

void printSymbolTable(SymbolTable * st) {
	if (st) {
		printScope(st -> root, 0);
	} else fprintf(stderr, "Symbol table has no entries.\n");
}

void printScope(Scope * scope, int depth) {
	int i = 0;
	if (scope) {
		printScopeType(scope -> type, depth);

		for( i = 0 ; i < depth ; i++) printf("\t");
		printf(" [symbols (%d)]\n", scope -> symbolCount);

		printSymbols(scope -> symbols, depth);	

		for( i = 0 ; i < depth ; i++) printf("\t");
		printf(" [sub-scopes (%d)]\n", scope -> subscopeCount);

		for (i = 0 ; i < MAX_SUBSCOPES && scope->subscopes[i] != NULL ; i++) {
			printScope(scope -> subscopes[i], depth + 1);
		}

		printf("\n");
	}
}

void printScopeType(ScopeType type, int depth) {
	int i = 0;
	char buf[128];

	for( i = 0 ; i < depth ; i++) printf("\t");

	switch(type) {
		case SCOPE_SELECTION:
			strcpy(buf, "SELECTION STMT");
			break;
		case SCOPE_LOOP:
			strcpy(buf, "LOOP");
			break;
		case SCOPE_FUNCTION:
			strcpy(buf, "FUNCTION");
			break;
		case SCOPE_FILE:	
			strcpy(buf, "FILE");
			break;
		default:
			strcpy(buf, "?? unknown scope type ??");
	}

	printf("%s\n", buf);
}

void printSymbols(Symbol ** symbols, int depth) {
	if (symbols && symbols[0]) {
		int i = 0, j = 0;
		char buf[10];
		for (i = 0 ; symbols && symbols[i] != NULL ; i++) {
			for(j = 0 ; j < depth ; j++) {
				printf("\t");
			}

			if (symbols[i] -> isInt == 1) {
				strcpy(buf, "int");
			} else {
				strcpy(buf, "void");
			}

			if (symbols[i] -> type == SYMBOL_FUNCTION) {
				printf("   - Function (%s %s)\n", buf, symbols[i] -> name);
			} else if (symbols[i] -> type == SYMBOL_VAR) {
				printf("   - Variable (%s %s)\n", buf, symbols[i] -> name);
			} else if (symbols[i] -> type == SYMBOL_FPARAM) {
				printf("   - Parameter (%s %s)\n", buf, symbols[i] -> name);
			} else {
				printf("?? UNKNOWN ??\n");
			}
		}
	} 
}