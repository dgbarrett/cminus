#include "SyntaxTree.h"

/* base builder */
ASTNode * new_ASTNode( ASTNodeType ntype ) {
	ASTNode * node = malloc(sizeof(*node));

	node -> type = ntype;
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
		if (!(parent -> children[i])) {
			parent -> children[i] = child;
			return;
		}
	}

	/* Error case */
	fprintf(stderr, "Parent node is already full\n");
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
/* node builders */

ASTNode * Program( ASTNode * program ) {
	if ( program ) return program;
	else {
		ASTNode * program = new_ASTNode(PROGRAM);
		return program;
	}
}

ASTNode * Variable() {
	return NULL;
}

ASTNode * VariableArray() {
	return NULL;
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
	ASTNode * node = new_ASTNode(IDENTIFIER);
	ASTNode_setStrValue(node, strtype);
	return node;
}

ASTNode * Function() {
	return new_ASTNode(FUNCTION);
}

ASTNode * ParameterList() {
	return NULL;
}

ASTNode * Parameter() {
	return NULL;
}

ASTNode * ArrayParameter() {
	return NULL;
}

ASTNode * LocalVariables() {
	return NULL;
}

ASTNode * StatementList() {
	return NULL;
}

ASTNode * CompoundStatement() {
	return NULL;
}

ASTNode * Expression() {
	return NULL;
}

ASTNode * IfStatement() {
	return NULL;
}

ASTNode * WhileLoop() {
	return NULL;
}

ASTNode * ReturnStatement() {
	return NULL;
}

ASTNode * VariableArrayElement() {
	return NULL;
}

ASTNode * FunctionCall() {
	return NULL;
}

ASTNode * ArgumentList() {
	return NULL;
}

ASTNode * Operator_Plus() {
	return NULL;
}

ASTNode * Operator_Minus() {
	return NULL;
}

ASTNode * Operator_Div() {
	return NULL;
}

ASTNode * Operator_Muls() {
	return NULL;
}

ASTNode * Operator_Greater() {
	return NULL;
}

ASTNode * Operator_GreaterEqual() {
	return NULL;
}

ASTNode * Operator_Less() {
	return NULL;
}
ASTNode * Operator_LessEqual() {
	return NULL;
}

ASTNode * Operator_Assignment() {
	return NULL;
}

ASTNode * Operator_Equal() {
	return NULL;
}

ASTNode * Operator_NotEqual() {
	return NULL;
}

void Program_appendDeclaration( ASTNode * program, ASTNode * declaration) {
	if (program && declaration) {
		ASTNode_appendChild(program, declaration);
	}
	return;
}

void Variable_setType( ASTNode * variable, ASTNode * type) {
	return;
}

void Variable_setIdentifier( ASTNode * variable, ASTNode * id) {
	return;
}

void VariableArray_setType( ASTNode * variablearr, ASTNode * type) {
	return;
}

void VariableArray_setIdentifier( ASTNode * variablearr, ASTNode * id) {
	return;
}

void VariableArray_setSize( ASTNode * variablearr, ASTNode * type) {
	return;
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
	return;
}

void Parameter_setType( ASTNode * param, ASTNode * type) {
	return;
}

void Parameter_setIdentifier( ASTNode * param, ASTNode * id) {
	return;
}

void ArrayParameter_setType( ASTNode * arrparam, ASTNode * type) {
	return;
}

void ArrayParameter_setIdentifier( ASTNode * arrparam, ASTNode * id) {
	return;
}


void LocalVariables_append( ASTNode * vars, ASTNode * var) {
	return;
}

void StatementList_append( ASTNode * stmtlist, ASTNode * stmt) {
	return;
}

void CompoundStatement_setLocalVars( ASTNode * stmt, ASTNode * vars) {
	return;
}

void CompoundStatement_setStatementList( ASTNode * stmt, ASTNode * stmtlist) {
	return;
}

void IfStatement_setCondition( ASTNode * ifstmt, ASTNode * condition) {
	return;
}

void IfStatement_setBody( ASTNode * ifstmt, ASTNode * body) {
	return;
}

void IfStatement_setElseBody( ASTNode * ifstmt, ASTNode * elsebody) {
	return;
}

void WhileLoop_setCondition( ASTNode * whilestmt, ASTNode * condition ) {
	return;
}

void WhileLoop_setBody( ASTNode * whilestmt, ASTNode * body ) {
	return;
}

void ReturnStatement_setReturnValue( ASTNode * returnstmt, ASTNode * returnvalue) {
	return;
}

void Expression_setType( ASTNode * expression, ASTNode * exptype) {
	return;
}

void Expression_setVariable( ASTNode * expression, ASTNode * variable) {
	return;
}

void Expression_setValue( ASTNode * expression, ASTNode * value) {
	return;
}

void Expression_setSubExpressions( ASTNode * expression, ASTNode * subexpr1, ASTNode * subexpr2) {
	return;
}

void VariableArrayElement_setParentArray( ASTNode * arrayelem, ASTNode * parent) {
	return;
}

void VariableArrayElement_setIndex( ASTNode * arrayelem, ASTNode * index) {
	return;
}

void FunctionCall_functionCalled( ASTNode * function, ASTNode * name) {
	return;
}

void FunctionCall_arguments(ASTNode * function, ASTNode * args) {
	return;
}

void ArgumentList_append(ASTNode * arglist, ASTNode * arg) {
	return;
}

void printNodeType(ASTNode * node) {
	switch( node -> type ) {
		case PROGRAM:
			printf("PROGRAM\n");
			break;
		case VAR_DECLARATION:
			printf("Variable Declaration\n");
			break;
		case VAR_ARRAY_DECLARATION:
			printf("Array Declaration\n");
			break;
		case IDENTIFIER:
			printf("Identifier\n");
			break;
		case NUMBER:
			printf("Number\n");
			break;
		case FUNCTION:
			printf("Function Declaration\n");
			break;
		default:
			printf("Unrecongnized Node Type\n");
			break;
	}
}

void printSyntaxTree(ASTNode * root) {
	printNodeType(root);

	int i=0;
	for (i=0 ; i<MAX_CHILDREN ; i++) {
		if (root -> children[i]) {
			printNodeType(root->children[i]);
		} else break;
	}

}













