#include "symbtable_print.h"

#include "hashtable.h"

#include <stdio.h>
#include <string.h>

#define SYMTABLE_OUT stdout

void printScope 	(Scope * scope, int depth);
void printScopeType	(ScopeType type, int depth);
void printSymbols	(Symbol ** symbols, int depth);
void pad			(int depth);

/*
	Function: printSymbolTable
		Prints the entire SymbolTable.
*/
void printSymbolTable(SymbolTable * st) {
	if (st) {
		printf("SYMBOL TABLE\n");
		printf("------------\n");
		printScope(st -> root, 0);
		printf("\n");
	} else fprintf(stderr, "Symbol table has no entries.\n");
}

/*
	Function: printScope
		Prints the symbols and subscopes contained within a scope.
		Indented to a depth of depth.
*/
void printScope(Scope * scope, int depth) {
	int i = 0;
	if (scope) {
		/* Print the type of the scope */
		printScopeType(scope -> type, depth);

		/* Print new symbols defined in the scope. */
		pad(depth);
		printf("    [new symbols (%d)]\n", scope -> symbolCount);
		printSymbols(scope -> symbols, depth);	

		/* Print all the symbols accessible in the scope */
		Symbol ** allsymbols = HashTable_getSymbols(scope -> allsymbols);
		pad(depth);
		fprintf(SYMTABLE_OUT, "    [all symbols (%d)]\n", scope->allsymbols->size);
		printSymbols(allsymbols, depth);

		/* Print the subscopes of the Scope */
		pad(depth);
		printf("    [sub-scopes (%d)]\n", scope -> subscopeCount);
		for (i = 0 ; i < MAX_SUBSCOPES && scope->subscopes[i] != NULL ; i++) {
			printScope(scope -> subscopes[i], depth + 1);
		}

	}
}

/*
	Function: printScopeType
		Prints the value of a ScopeType.
*/
void printScopeType(ScopeType type, int depth) {
	char buf[128];

	strcpy(buf, "[SCOPE] ");

	pad(depth);

	switch(type) {
		case SCOPE_SELECTION:
			strcat(buf, "SELECTION STMT");
			break;
		case SCOPE_LOOP:
			strcat(buf, "LOOP");
			break;
		case SCOPE_FUNCTION:
			strcat(buf, "FUNCTION");
			break;
		case SCOPE_FILE:	
			strcat(buf, "FILE");
			break;
		case SCOPE_STDLIB:
			strcat(buf, "STDLIB");
			break;
		default:
			strcat(buf, "?? unknown scope type ??");
	}

	printf("%s\n", buf);
}

/*
	Function: printSymbols
		Prints an array of Symbols. Indented to depth.
*/
void printSymbols(Symbol ** symbols, int depth) {
	if (symbols && symbols[0]) {
		int i = 0;
		char buf[10];
		for (i = 0 ; symbols && symbols[i] != NULL ; i++) {
			pad(depth);

			/* Copy type of symbol into buf */
			if (symbols[i] -> datatype == TYPE_INT || symbols[i] -> datatype == TYPE_INTARR) {
				strcpy(buf, "int");
			} else {
				strcpy(buf, "void");
			}

			/* Print symbol */
			if (symbols[i] -> type == SYMBOL_FUNCTION) {
				printf("      - Function (%s %s)", buf, symbols[i] -> name);
			} else if (symbols[i] -> type == SYMBOL_VAR) {
				printf("      - Variable (%s %s)", buf, symbols[i] -> name);
			} else if (symbols[i] -> type == SYMBOL_FPARAM) {
				printf("      - Parameter (%s %s)", buf, symbols[i] -> name);
			} else if (symbols[i] -> type == SYMBOL_FARRAYPARAM) {
				printf("      - Array Parameter (%s %s)", buf, symbols[i] -> name);
			} else if (symbols[i] -> type == SYMBOL_ARRAY) {
				printf("      - Array (%s %s[%d])", buf, symbols[i] -> name, symbols[i] -> arrlen);
			} else {
				printf("?? UNKNOWN ??");
			}

			if (symbols[i] -> dmem) {
				printf("(DMemSymbol found.)\n");
			} else printf("\n");
		}
	} 
}

/*
	Function: pad
		Prints depth number of tabs without a newline.
*/
void pad(int depth) {
	int i = 0;
	char buf[128];
	for( i = 0 ; i < depth && i < 127 ; i++) buf[i] = '\t';
	buf[i] = '\0';
	fprintf(SYMTABLE_OUT, "%s", buf);
}