#include "SyntaxTree.h"


/* node builders */

ASTNode * Program() {
	return NULL;
}

ASTNode * Variable() {
	return NULL;
}

ASTNode * VariableArray() {
	return NULL;
}

ASTNode * Identifier() {
	return NULL;
}

ASTNode * Number() {
	return NULL;
}

ASTNode * Type() {
	return NULL;
}

ASTNode * Function() {
	return NULL;
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

void Function_setReturnType( ASTNode * function, ASTNode * rettype) {
	return;
}

void Function_setIdentifier( ASTNode * function, ASTNode * id) {
	return;
}

void Function_setParameters( ASTNode * function, ASTNode * params) {
	return;
}

void Function_setDefinition( ASTNode * function, ASTNode * def) {
	return;
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













