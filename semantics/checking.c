#include "checking.h"

#include "../parse/ErrorManager.h"

void 	checkForRedefinedVariables 		(ErrorList * errlist, Scope * scope);
void 	checkExpressions				(ErrorList * errlist, ASTNode * node);
void 	generateCompleteSymbolListing	(Scope * scope, Scope * parent);
void 	checkScopeForRedefinedVariables	(ErrorList * errlist, Scope * scope);
SymbolDataType 	evaluateType					(ErrorList * errlist, ASTNode * subexpr);

void semanticAnalysis(ASTNode * ast, SymbolTable * symbtable) {
	/* List of semantic errors */
	ErrorList * semanticErrors = new_ErrorList();

	checkScopeForRedefinedVariables(semanticErrors, symbtable -> root);
	checkExpressions(semanticErrors, ast);

	/* Print any errors */
	ErrorList_print(semanticErrors);
}

/*
	Function: checkScopeForRedefinedVariables
		Checks a scope and all its subscopes for redefined variables.
*/
void checkScopeForRedefinedVariables(ErrorList * errlist, Scope * scope) {
	int i = 0;
	if (scope) {
		checkForRedefinedVariables(errlist, scope);	

		for (i=0 ; i<scope->subscopeCount ; i++) {
			checkScopeForRedefinedVariables(errlist, scope -> subscopes[i]);
		}
	}
}

/*
	Function: checkForRedefinedVariables
		Check a Scope to see if duplicate symbols are defined within it.  If 
		there are duplicates, store a new Error in the ErrorList.  The function
		also generates a SymbolHashTable in the Scope storing all symbols 
		accessible from the Scope. 
*/
void checkForRedefinedVariables(ErrorList * errlist, Scope * scope) {
	int i = 0;
	/* Check for symbols defined in the scope that conflict */
	for (i = 0 ; i < scope -> symbolCount ; i++) {
		if(!HashTable_insert(scope -> allsymbols, scope -> symbols[i])) {
			char buf[128];
			Symbol * new = scope -> symbols[i];
			Symbol * old = HashTable_get(scope -> allsymbols, scope -> symbols[i] -> name);

			if (old -> linenum == 0) {
				sprintf(buf,"Symbol '%s' redefined.\n\t '%s' is defined as part of the C- standard library.\n",new->name, new->name);
			} else {
				sprintf(buf,"Symbol '%s' redefined.\n\t Previous definition exists at line %d.\n",new->name, old->linenum);
			}
			ErrorList_insert(errlist, new_Error(buf, new->linenum , 0));
		}
	}

	/* Roll up complete symbol table. */
	generateCompleteSymbolListing(scope, scope -> parent);
}

/*
	Function: generateCompleteSymbolListing
		Traverse from a given scope to the root scope collecting a listing 
		of symbols accessible from scope.  The most local symbol overrides any
		others above it with the same name.
*/
void generateCompleteSymbolListing(Scope * scope, Scope * parent) {
	if (parent) {
		int i = 0;
		for (i = 0 ; i < parent -> symbolCount ; i++) {
			HashTable_insert(scope -> allsymbols, parent -> symbols[i]);
		}

		generateCompleteSymbolListing(scope, parent -> parent);
	}
}

void checkExpressions(ErrorList * errlist, ASTNode * node) {
	if (node && node -> type == EXPRESSION) {
		ASTNode * subexpr1 = node -> children[0];
		ASTNode * subexpr2 = node -> children[1];

		SymbolDataType t1 = evaluateType(errlist, subexpr1);
		SymbolDataType t2 = evaluateType(errlist, subexpr2);

		if (t1 != t2) {
			ErrorList_insert(errlist, new_Error("TEST", 0, 0));
		}
	} else {
		int i;
		for (i = 0; node -> children[i] != NULL ; i++) {
			checkExpressions(errlist, node -> children[i]);
		}
	}
	
}

SymbolDataType evaluateType(ErrorList * errlist, ASTNode * expr) {
	if (errlist && expr) {
		SymbolHashTable * enclosingScope = ASTNode_getEnclosingScope(expr);
		Symbol * temp = NULL;
		SymbolDataType t1,t2;

		switch(expr -> type) {
			case NUMBER:
				return TYPE_INT;
			case IDENTIFIER:
			case FUNCTION_CALL:
				temp = HashTable_get(enclosingScope, expr -> value.str);
				if ( temp ) {
					return temp -> datatype;
				} else {
					printf("Symbol does not exist\n");
					exit(0);
				}
				
			case VAR_ARRAY_ELEMENT:
				printf("var array elem\n");
				temp = HashTable_get(enclosingScope, expr -> children[0] -> value.str);
				return temp -> datatype;
			case EXPRESSION:
				printf("expression inside\n");
				t1 = evaluateType(errlist, expr->children[0]);
				t2 = evaluateType(errlist, expr->children[1]);

				if (t1 != t2) {
					ErrorList_insert(errlist, new_Error("Types of expression do not match", 0, 0));
					return TYPE_INT;
				} else return t1;
			default:
				printf("Shouldn't be here\n");
				break;
		}
	}
	return TYPE_INT;
}