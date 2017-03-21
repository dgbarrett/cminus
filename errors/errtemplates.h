#ifndef __ERR_TEMPLATES_H__
#define __ERR_TEMPLATES_H__

char * ErrTemplate_MismatchedExprType(char * exprType, char * name1, char * type1, char * name2, char * type2);
char * ErrTemplate_UndefinedSymbol(char * name);
char * ErrTemplate_InvalidArrayAccessType(char * arrname, char * access, char * accessType);
char * ErrTemplate_InvalidConditionType(char * stmtType, char * condition, char * condType);

#endif