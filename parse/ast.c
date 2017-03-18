#include "ast.h"

/* Base Node Type */

ASTNode * new_ASTNode( ASTNodeType ntype ) {
	ASTNode * node = malloc(sizeof(*node));

	node -> type = ntype;
	node -> linenum = -1;
	node -> children = calloc(MAX_CHILDREN, sizeof(*(node->children)));

	int i;
	for ( i = 0 ; i < MAX_CHILDREN ; i++ ) {
		node -> children[i] = NULL;
	}

	return node;
}

void ASTNode_appendChild( ASTNode * parent, ASTNode * child ) {
	int i;
	for ( i = 0 ; i < MAX_CHILDREN ; i++ ) {
		if (parent -> children[i] == NULL) {
			parent -> children[i] = child;
			return;
		}
	}

	/* Error case */
}

void ASTNode_setLineNum( ASTNode * node, int line) {
	if (node) node -> linenum = line;
}

int ASTNode_getLineNum( ASTNode * node ) {
	if (node) return node -> linenum;
	else return -1;
}

void ASTNode_setStrValue( ASTNode * parent, char * value) {
	if (parent) {
		parent -> value.str = calloc(strlen(value) + 1, sizeof(*value));
		strcpy(parent->value.str, value);
	}

}

void ASTNode_setIntValue( ASTNode * parent, int value) {
	if (parent) {
		parent -> value.num = value;
	}
}

void ASTNode_setOperatorValue( ASTNode * parent, Operator value){
	if (parent) {
		parent -> value.operation = value;
	}
}

/******************** Node Builders (makes the code pretty) ********************/

ASTNode * Program( ASTNode * program ) {
	if ( program ) return program;
	else {
		ASTNode * program = new_ASTNode(PROGRAM);
		return program;
	}
}

ASTNode * Variable() {
	return new_ASTNode(VARIABLE);
}

ASTNode * VariableDeclaration() {
	return new_ASTNode(VAR_DECLARATION);
}

ASTNode * VariableArrayDeclaration() {
	return new_ASTNode(VAR_ARRAY_DECLARATION);
}

ASTNode * Identifier( char * strid ) {
	ASTNode * node = new_ASTNode(IDENTIFIER);
	ASTNode_setStrValue(node, strid);
	return node;
}

ASTNode * Number( char * strnum ) {
	ASTNode * node = new_ASTNode(NUMBER);
	ASTNode_setIntValue(node, atoi(strnum));
	return node;
}

ASTNode * Type( char * strtype ) {
	ASTNode * node = new_ASTNode(TYPE);
	ASTNode_setStrValue(node, strtype);
	return node;
}

ASTNode * Function() {
	return new_ASTNode(FUNCTION);
}

ASTNode * ParameterList( ASTNode * paramlist ) {
	if (paramlist) return paramlist;
	else return new_ASTNode(PARAMETER_LIST);
}

ASTNode * Parameter() {
	return new_ASTNode(PARAMETER);
}

ASTNode * ArrayParameter() {
	return new_ASTNode(ARRAY_PARAMETER);
}

ASTNode * LocalVariables( ASTNode * localvars) {
	if (localvars) return localvars;
	else return new_ASTNode(LOCAL_VARS);
}

ASTNode * StatementList( ASTNode *stmtlist ) {
	if (stmtlist) return stmtlist;
	else return new_ASTNode(STATEMENT_LIST);
}

ASTNode * CompoundStatement() {
	return new_ASTNode(COMPOUND_STATEMENT);
}

ASTNode * Expression() {
	return new_ASTNode(EXPRESSION);
}

ASTNode * IfStatement() {
	return new_ASTNode(IF_STATEMENT);
}

ASTNode * WhileLoop() {
	return new_ASTNode(WHILE_LOOP);
}

ASTNode * ReturnStatement() {
	return new_ASTNode(RETURN_STATEMENT);
}

ASTNode * VariableArrayElement() {
	return new_ASTNode(VAR_ARRAY_ELEMENT);
}

ASTNode * FunctionCall() {
	return new_ASTNode(FUNCTION_CALL);
}

ASTNode * ArgumentList( ASTNode * arglist ) {
	if (arglist) return arglist;
	else return new_ASTNode(ARGUMENT_LIST);
}

ASTNode * Operation( char * tokenString ) {
	ASTNode * node = new_ASTNode(_OPERATION);

	if ( strcmp(tokenString, "=") == 0) {
		ASTNode_setOperatorValue(node, ASSIGN);
	} else if ( strcmp(tokenString, "!=") == 0) {
		ASTNode_setOperatorValue(node, NEQ);
	} else if ( strcmp(tokenString, "==") == 0) {
		ASTNode_setOperatorValue(node, EQ);
	} else if ( strcmp(tokenString, ">=") == 0) {
		ASTNode_setOperatorValue(node, GE);
	} else if ( strcmp(tokenString, ">") == 0) {
		ASTNode_setOperatorValue(node, GT);
	} else if ( strcmp(tokenString, "<=") == 0) {
		ASTNode_setOperatorValue(node, LE);
	} else if ( strcmp(tokenString, "<") == 0) {
		ASTNode_setOperatorValue(node, LT);
	} else if ( strcmp(tokenString, "+") == 0) {
		ASTNode_setOperatorValue(node, ADD);
	} else if ( strcmp(tokenString, "-") == 0) {
		ASTNode_setOperatorValue(node, SUB);
	} else if ( strcmp(tokenString, "/") == 0) {
		ASTNode_setOperatorValue(node, DIV);
	}  else if ( strcmp(tokenString, "*") == 0) {
		ASTNode_setOperatorValue(node, MUL);
	} else {
		fprintf(stderr, "Unrecongnized operator value (%s)\n", tokenString);
	}

	return node; 
}

/********* Node Modifiers ***********/
void Program_appendDeclaration( ASTNode * program, ASTNode * declaration) {
	if (program && declaration) {
		ASTNode_appendChild(program, declaration);
	}
	return;
}

void Variable_setType( ASTNode * variable, ASTNode * type) {
	ASTNode_appendChild(variable, type);
}

void Variable_setIdentifier( ASTNode * variable, ASTNode * id) {
	ASTNode_appendChild(variable, id);
}

void VariableDeclaration_setType( ASTNode * variable, ASTNode * type) {
	ASTNode_appendChild(variable, type);
}

void VariableDeclaration_setIdentifier( ASTNode * variable, ASTNode * id) {
	ASTNode_appendChild(variable, id);
}

void VariableArrayDeclaration_setType( ASTNode * variablearr, ASTNode * type) {
	ASTNode_appendChild(variablearr, type);
}

void VariableArrayDeclaration_setIdentifier( ASTNode * variablearr, ASTNode * id) {
	ASTNode_appendChild(variablearr, id);
}

void VariableArrayDeclaration_setSize( ASTNode * variablearr, ASTNode * size) {
	ASTNode_appendChild(variablearr, size);
}

void Function_setReturnType( ASTNode * function, ASTNode * rettype) {
	ASTNode_appendChild(function, rettype);
}

void Function_setIdentifier( ASTNode * function, ASTNode * id) {
	ASTNode_appendChild(function, id);
}

void Function_setParameters( ASTNode * function, ASTNode * params) {
	ASTNode_appendChild(function, params);
}

void Function_setDefinition( ASTNode * function, ASTNode * def) {
	ASTNode_appendChild(function, def);
}

void ParameterList_append( ASTNode * paramlist, ASTNode * param) {
	ASTNode_appendChild(paramlist, param);
}

void Parameter_setType( ASTNode * param, ASTNode * type) {
	ASTNode_appendChild(param, type);
}

void Parameter_setIdentifier( ASTNode * param, ASTNode * id) {
	ASTNode_appendChild(param, id);
}

void ArrayParameter_setType( ASTNode * arrparam, ASTNode * type) {
	ASTNode_appendChild(arrparam, type);
}

void ArrayParameter_setIdentifier( ASTNode * arrparam, ASTNode * id) {
	ASTNode_appendChild(arrparam, id);
}

void LocalVariables_append( ASTNode * vars, ASTNode * var) {
	ASTNode_appendChild(vars, var);
}

void StatementList_append( ASTNode * stmtlist, ASTNode * stmt) {
	ASTNode_appendChild(stmtlist, stmt);
}

void CompoundStatement_setLocalVars( ASTNode * stmt, ASTNode * vars) {
	ASTNode_appendChild(stmt, vars);
}

void CompoundStatement_setStatements( ASTNode * stmt, ASTNode * stmtlist) {
	int i;
	for (i = 0; i < MAX_CHILDREN ; i++) {
		if (stmtlist -> children[i]) {
			ASTNode_appendChild(stmt, stmtlist -> children[i]);
		} else {
			break;
		}
	}
	
}

void IfStatement_setCondition( ASTNode * ifstmt, ASTNode * condition) {
	ASTNode_appendChild(ifstmt, condition);
}

void IfStatement_setBody( ASTNode * ifstmt, ASTNode * body) {
	ASTNode_appendChild(ifstmt, body);
}

void IfStatement_setElseBody( ASTNode * ifstmt, ASTNode * elsebody) {
	ASTNode_appendChild(ifstmt, elsebody);
}

void WhileLoop_setCondition( ASTNode * whilestmt, ASTNode * condition ) {
	ASTNode_appendChild(whilestmt, condition);
}

void WhileLoop_setBody( ASTNode * whilestmt, ASTNode * body ) {
	ASTNode_appendChild(whilestmt, body);
}

void ReturnStatement_setReturnValue( ASTNode * returnstmt, ASTNode * returnvalue) {
	ASTNode_appendChild(returnstmt, returnvalue);
}

void Expression_setType( ASTNode * expression, ASTNode * exptype) {
	ASTNode_setOperatorValue(expression, exptype -> value.operation);
}

void Expression_setVariable( ASTNode * expression, ASTNode * variable) {
	ASTNode_appendChild(expression, variable);
}

void Expression_setValue( ASTNode * expression, ASTNode * value) {
	ASTNode_appendChild(expression, value);
}

void Expression_setSubExpressions( ASTNode * expression, ASTNode * subexpr1, ASTNode * subexpr2) {
	ASTNode_appendChild(expression, subexpr1);
	ASTNode_appendChild(expression, subexpr2);
}

void VariableArrayElement_setParentArray( ASTNode * arrayelem, ASTNode * parent) {
	ASTNode_appendChild(arrayelem, parent);
}

void VariableArrayElement_setIndex( ASTNode * arrayelem, ASTNode * index) {
	ASTNode_appendChild(arrayelem, index);
}

void FunctionCall_functionCalled( ASTNode * function, ASTNode * name) {
	ASTNode_appendChild(function, name);
}

void FunctionCall_arguments(ASTNode * function, ASTNode * args) {
	ASTNode_appendChild(function, args);
}

void ArgumentList_append(ASTNode * arglist, ASTNode * arg) {
	ASTNode_appendChild(arglist, arg);
}











