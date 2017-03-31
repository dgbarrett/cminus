#include "ast_print.h"

void _printSyntaxTree(ASTNode * root, ASTNodeType parentType, int index, int depth);
void printIdentifier(ASTNode * node);
void printStrNodeValue(ASTNode * node, char * nodename) ;
void printIntNodeValue(ASTNode * node, char * nodename);
void printNodeType(ASTNode * node) ;
void printRelativePartName(ASTNodeType type, int index) ;

void printSyntaxTree(ASTNode * root) {
	printf("SYNTAX TREE\n");
	printf("-----------\n");
	_printSyntaxTree(root, NIL, 0, 0);
	printf("\n");
}

void _printSyntaxTree(ASTNode * root, ASTNodeType parentType, int index, int depth) {
	if (root) {
		int i =0;
		for (i=0 ; i < depth ; i++) {
			printf("\t");
		}

		printRelativePartName(parentType, index);
		printNodeType(root);

		for (i=0 ; i<MAX_CHILDREN ; i++) {
			if (root -> children[i]) {
				_printSyntaxTree(root->children[i], root->type, i, depth+1);
			} else return;
		}	
	}
}

void printExpression(ASTNode * node) {
	char operator[3];

	switch( node -> value.operation ) {
		case GE:
			strcpy(operator,">=");
			break;
		case GT:
			strcpy(operator,">");
			break;
		case LE:
			strcpy(operator,"<=");
			break;
		case LT:
			strcpy(operator,"<");
			break;
		case EQ:
			strcpy(operator,"==");
			break;
		case NEQ:
			strcpy(operator,"!=");
			break;
		case ASSIGN:
			strcpy(operator,"=");
			break;
		case ADD:
			strcpy(operator,"+");
			break;
		case SUB:
			strcpy(operator,"-");
			break;
		case DIV:
			strcpy(operator,"/");
			break;
		case MUL:
			strcpy(operator,"*");
			break;
		default:
			strcpy(operator,"???");
			break;
	}

	printf("Expression (%s)\n", operator);
}

void printIdentifier(ASTNode * node) {
	printf("Identifier (\"%s\")\n", node -> value.str);
}

void printStrNodeValue(ASTNode * node, char * nodename) {
	printf("%s (%s)\n", nodename, node -> value.str);
}

void printIntNodeValue(ASTNode * node, char * nodename) {
	printf("%s (%d)\n", nodename, node -> value.num);
}

void printNodeType(ASTNode * node) {
	if (node) {
		switch( node -> type ) {
			case PROGRAM:
				printf("PROGRAM ");
				if (node -> scope) {
					printf("(NEW SCOPE)\n");
				}else {
					printf("\n");
				}
				break;
			case VAR_DECLARATION:
				printf("Variable Declaration\n");
				break;
			case VAR_ARRAY_DECLARATION:
				printf("Array Declaration\n");
				break;
			case IDENTIFIER:
				printIdentifier(node);
				break;
			case NUMBER:
				printIntNodeValue(node, "Number");
				break;
			case FUNCTION_DECLARATION:
				printf("Function Declaration\n");
				break;
			case PARAMETER_LIST:
				printf("ParameterList\n");
				break;
			case TYPE:
				printStrNodeValue(node, "Type");
				break;
			case PARAMETER:
				printf("Parameter\n");
				break;
			case ARRAY_PARAMETER:
				printf("Array Parameter\n");
				break;
			case LOCAL_VARS:
				printf("Local Variables\n");
				break;
			case STATEMENT_LIST:
				printf("Statement List\n");
				break;
			case COMPOUND_STATEMENT:
				printf("Compound Statement ");
				if (node -> scope) {
					printf("(NEW SCOPE)\n");
				}else {
					printf("\n");
				}
				break;
			case EXPRESSION:
				printExpression(node);
				break;
			case IF_STATEMENT:
				printf("If Statement\n");
				break;
			case WHILE_LOOP:
				printf("While Loop\n");
				break;
			case RETURN_STATEMENT:
				printf("Return Statement\n");
				break;
			case VARIABLE:
				printf("Variable\n");
				break;
			case VAR_ARRAY_ELEMENT:
				printf("Variable Array Element\n");
				break;
			case ARGUMENT_LIST:
				printf("Argument List\n");
				break;
			case FUNCTION_CALL:
				printf("Function Call\n");
				break;
			case SYNTAX_ERROR:
				printf("Syntax Error\n");
				break;
			case _OPERATION:
				printf("Operation (temp)\n");
				printf("\tTemp node type (_OPERATION) should not be used in final tree\n");
				break;
			default:
				printf("Unrecongnized Node Type\n");
				break;
		}
	}
	
}

void printRelativePartName(ASTNodeType type, int index) {
	char buf[50];
	char bufnum[10];
	switch(type) {
		case IF_STATEMENT:
			switch(index) {
				case 0:
					printf("%s ", "[CONDITION]");
					break;
				case 1:
					printf("%s ", "[IF-BODY]");
					break;
				case 2:
					printf("%s ", "[ELSE-BODY]");
					break;
				default:
					printf("[IF_STATEMENT *error*] ");
			}
			break;
		case EXPRESSION:
			switch(index) {
				case 0:
					printf("%s ", "[SUB EXPR 1]");
					break;
				case 1:
					printf("%s ", "[SUB EXPR 2]");
					break;
			}
			break;
		case FUNCTION_CALL:
			switch(index) {
				case 0:
					printf("%s ", "[NAME]");
					break;
				case 1:
					printf("%s ", "[ARGUMENTS]");
					break;
			}
			break;
		case ARGUMENT_LIST:
			strcpy(buf, "[ARG ");
			sprintf(bufnum, "%d", index+1);
			strcat(buf, bufnum);
			strcat(buf, "]");
			printf("%s ", buf);
			break;
		case PROGRAM:
			strcpy(buf, "[DECLARATION ");
			sprintf(bufnum, "%d", index+1);
			strcat(buf, bufnum);
			strcat(buf, "]");
			printf("%s ", buf);
			break;
		case FUNCTION_DECLARATION:
			switch(index) {
				case 0:
					printf("%s ", "[RETURN TYPE]");
					break;
				case 1:
					printf("%s ", "[NAME]");
					break;
				case 2:
					printf("%s ", "[PARAMETERS]");
					break;
				case 3:
					printf("%s ", "[DEFINITION]");
					break;
			}
			break;
		case PARAMETER_LIST:
			strcpy(buf, "[PARAMETER ");
			sprintf(bufnum, "%d", index+1);
			strcat(buf, bufnum);
			strcat(buf, "]");
			printf("%s ", buf);
			break;
		case PARAMETER:
			switch(index) {
				case 0:
					printf("%s ", "[PARAMETER TYPE]");
					break;
				case 1:
					printf("%s ", "[PARAMETER NAME]");
					break;
			}
			break;
		case COMPOUND_STATEMENT:
			strcpy(buf, "[SUB-STATMENT ");
			sprintf(bufnum, "%d", index+1);
			strcat(buf, bufnum);
			strcat(buf, "]");
			printf("%s ", buf);
			break;
		case RETURN_STATEMENT:
			printf("[RETURN VALUE] ");
			break;
		case WHILE_LOOP:
			switch(index) {
				case 0:
					printf("%s ", "[CONDITION]");
					break;
				case 1:
					printf("%s ", "[LOOP BODY]");
					break;
			}
			break;
		case VAR_DECLARATION:
			switch(index) {
				case 0:
					printf("%s ", "[VAR TYPE]");
					break;
				case 1:
					printf("%s ", "[VAR NAME]");
					break;
			}
			break;
		case LOCAL_VARS:
			strcpy(buf, "[LOCAL VAR ");
			sprintf(bufnum, "%d", index+1);
			strcat(buf, bufnum);
			strcat(buf, "]");
			printf("%s ", buf);
			break;
		default:
			break;
	}
}


