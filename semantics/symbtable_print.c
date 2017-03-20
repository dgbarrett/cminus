#include "symbtable_print.h"

#include <stdio.h>

void printScope(Scope * scope, int depth);
void printScopeType(ScopeType type, int depth);
void printSymbols(Symbol ** symbols, int depth);
void pad(int depth);

void HashTable_print(SymbolHashTable * ht, int depth) {
	Symbol ** arr = calloc(ht -> size, sizeof(*arr));
	int i = 0 ,index=0;
	for (i=0 ; i<ht->maxsize ; i++ ) {
		if (ht -> symbols[i] != NULL) {
			arr[index++] = ht -> symbols[i];
		}
	}

	pad(depth);
	printf(" {all symbols (%d)}\n", ht->size);

	printSymbols(arr, depth);
}

void printSymbolTable(SymbolTable * st) {
	if (st) {
		printScope(st -> root, 0);
	} else fprintf(stderr, "Symbol table has no entries.\n");
}

void printScope(Scope * scope, int depth) {
	int i = 0;
	if (scope) {
		printScopeType(scope -> type, depth);

		pad(depth);
		printf(" {new symbols (%d)}\n", scope -> symbolCount);

		printSymbols(scope -> symbols, depth);	

		HashTable_print(scope -> allsymbols, depth);

		pad(depth);
		printf(" {sub-scopes (%d)}\n", scope -> subscopeCount);

		for (i = 0 ; i < MAX_SUBSCOPES && scope->subscopes[i] != NULL ; i++) {
			printScope(scope -> subscopes[i], depth + 1);
		}

	}
}

void printScopeType(ScopeType type, int depth) {
	char buf[128];

	pad(depth);

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
		int i = 0;
		char buf[10];
		for (i = 0 ; symbols && symbols[i] != NULL ; i++) {
			pad(depth);

			/* Copy type of symbol into buf */
			if (symbols[i] -> isInt == 1) {
				strcpy(buf, "int");
			} else {
				strcpy(buf, "void");
			}

			/* Print symbol */
			if (symbols[i] -> type == SYMBOL_FUNCTION) {
				printf("   - Function (%s %s)\n", buf, symbols[i] -> name);
			} else if (symbols[i] -> type == SYMBOL_VAR) {
				printf("   - Variable (%s %s)\n", buf, symbols[i] -> name);
			} else if (symbols[i] -> type == SYMBOL_FPARAM) {
				printf("   - Parameter (%s %s)\n", buf, symbols[i] -> name);
			} else if (symbols[i] -> type == SYMBOL_FARRAYPARAM) {
				printf("   - Array Parameter (%s %s)\n", buf, symbols[i] -> name);
			} else if (symbols[i] -> type == SYMBOL_ARRAY) {
				printf("   - Array (%s %s[%d])\n", buf, symbols[i] -> name, symbols[i] -> arrlen);
			} else {
				printf("?? UNKNOWN ??\n");
			}
		}
	} 
}

void pad(int depth) {
	int i = 0;
	for( i = 0 ; i < depth ; i++) printf("\t");
}