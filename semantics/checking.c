#include "checking.h"

#include "../errors/ErrorManager.h"
#include "../parse/ast_print.h"

void 	checkForRedefinedVariables 		(ErrorList * errlist, Scope * scope);
void 	checkExpressionTypes				(ErrorList * errlist, ASTNode * node);
void checkConditionTypes(ErrorList * errlist, ASTNode * node);
void 	generateCompleteSymbolListing	(Scope * scope, Scope * parent);
void 	checkScopeForRedefinedVariables	(ErrorList * errlist, Scope * scope);
char *	getSubexpressionName(ASTNode * subexpr);
SymbolDataType 	evaluateType					(ErrorList * errlist, ASTNode * subexpr);

void semanticAnalysis(ASTNode * ast, SymbolTable * symbtable) {
	/* List of semantic errors */
	ErrorList * semanticErrors = new_ErrorList();

	checkScopeForRedefinedVariables(semanticErrors, symbtable -> root);
	checkExpressionTypes(semanticErrors, ast);
	checkConditionTypes(semanticErrors, ast);

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

void checkExpressionTypes(ErrorList * errlist, ASTNode * node) {
	if (node && node -> type == EXPRESSION) {
		ASTNode * subexpr1 = node -> children[0];
		ASTNode * subexpr2 = node -> children[1];

		SymbolDataType t1 = evaluateType(errlist, subexpr1);
		SymbolDataType t2 = evaluateType(errlist, subexpr2);

		if (t1 != t2) {
			ErrorList_insert(
				errlist, 
				new_Error(
					ErrTemplate_MismatchedExprType(
						Operator_toString(node -> value.operation),
						getSubexpressionName(subexpr1),
						SymbolDataType_toString(t1),
						getSubexpressionName(subexpr2),
						SymbolDataType_toString(t2)
					), 
					node->linenum, 
					0
				)
			);
		} 

		if (node -> parent -> type == IF_STATEMENT || node -> parent -> type == WHILE_LOOP) {
			if (node -> value.operation == ASSIGN && t1 != TYPE_INT) {
				ErrorList_insert(
					errlist, 
					new_Error(
						ErrTemplate_InvalidConditionType(
							(node -> parent -> type == IF_STATEMENT) ? "If" : "While loop",
							"",
							SymbolDataType_toString(t1)
						), 
						node->linenum, 
						0
					)
				);
			}
		}
	} else {
		int i;
		for (i = 0; node -> children[i] != NULL ; i++) {
			checkExpressionTypes(errlist, node -> children[i]);
		}
	}
}

void checkConditionTypes(ErrorList * errlist, ASTNode * node) {
	if (node){
		if (node -> type == IF_STATEMENT || node -> type == WHILE_LOOP) {
			ASTNode * condition = node -> children[0];

			if (condition -> type != EXPRESSION) {
				SymbolDataType condType = evaluateType(errlist, condition);
				if (condType != TYPE_INT)  {
					ErrorList_insert(
						errlist, 
						new_Error(
							ErrTemplate_InvalidConditionType(
								(node -> type == IF_STATEMENT) ? "If" : "While loop",
								"",
								SymbolDataType_toString(condType)
							), 
							condition->linenum, 
							0
						)
					);
				}
			}
		} else {
			int i;
			for (i = 0; node -> children[i] != NULL ; i++) {
				checkConditionTypes(errlist, node -> children[i]);
			}
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

char * getSubexpressionName(ASTNode * subexpr) {
	char * name = calloc(64, sizeof(*name));
	char * num = NULL;
	/* Error Handling */

	switch(subexpr -> type) {
		case IDENTIFIER:
			strcpy(name, subexpr -> value.str);
			return name;
		case NUMBER:
			num = calloc(16, sizeof(*num));
			sprintf(num, "%d", subexpr -> value.num);
			free(name);
			return num;
		case FUNCTION_CALL:
			strcpy(name, "call to ");
			strcat(name, subexpr -> children[0] -> value.str);
			return name;
		case EXPRESSION:
			free(name);
			return "subexpression";
		case VAR_ARRAY_ELEMENT:
			num = calloc(16, sizeof(*num));
			strcpy(name, subexpr -> children[0] -> value.str);
			sprintf(num, "[%s]", getSubexpressionName(subexpr -> children[1]));
			strcat(name, num);
			return name;
		default:
			free(name);
			return "??NoName??";
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
				temp = HashTable_get(enclosingScope, expr -> value.str);
				if ( temp ) {
					return temp -> datatype;
				} else {
					ErrorList_insert(
						errlist, 
						new_Error(
							ErrTemplate_UndefinedSymbol(
								expr -> value.str
							), 
							expr->linenum, 
							0
						)
					);
					return TYPE_INT;
				}
			case FUNCTION_CALL:
				return evaluateType(errlist, expr -> children[0]);
			case VAR_ARRAY_ELEMENT:
				/* evaluate the type of the element access */
				t1 = evaluateType(errlist, expr -> children[1]);

				if (t1 != TYPE_INT) {
					ErrorList_insert(
						errlist, 
						new_Error(
							ErrTemplate_InvalidArrayAccessType(
								getSubexpressionName(expr -> children[0]),
								getSubexpressionName(expr -> children[1]),
								SymbolDataType_toString(t1)
							), 
							expr->linenum, 
							0
						)
					);
				}

				temp = HashTable_get(enclosingScope, expr -> children[0] -> value.str);
				return SymbolDataType_parentType(temp -> datatype);
			case EXPRESSION:
				t1 = evaluateType(errlist, expr->children[0]);
				t2 = evaluateType(errlist, expr->children[1]);

				if (t1 != t2) {
					ErrorList_insert(
						errlist, 
						new_Error(
							ErrTemplate_MismatchedExprType(
								Operator_toString(expr -> value.operation),
								getSubexpressionName(expr -> children[0]),
								SymbolDataType_toString(t1),
								getSubexpressionName(expr -> children[1]),
								SymbolDataType_toString(t2)
							), 
							expr->linenum, 
							0
						)
					);
				} 

				if (expr -> value.operation == ASSIGN) {
					return t1;
				} else return TYPE_INT;
			default:
				break;
		}
	}
	return TYPE_INT;
}