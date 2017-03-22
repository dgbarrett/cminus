#ifndef __ERR_TEMPLATES_H__
#define __ERR_TEMPLATES_H__

char * ErrTemplate_MismatchedExprType(char * exprType, char * name1, char * type1, char * name2, char * type2);
char * ErrTemplate_UndefinedSymbol(char * name);
char * ErrTemplate_InvalidArrayAccessType(char * arrname, char * access, char * accessType);
char * ErrTemplate_InvalidConditionType(char * stmtType, char * condition, char * condType);
char * ErrTemplate_MismatchedSignature(char * funcName, char * expectedSig, char * actualSig);
char * ErrTemplate_RedefinedSymbol(char * name, int line);
char * ErrTemplate_RedefinedStdlibSymbol(char * name);
char * ErrTemplate_MissingReturnStmt(char * functionName);
char * ErrTemplate_UndefinedArray(char * name);
char * ErrTemplate_ArrayOutOfBounds(char * name, int index, int arrlen);

#endif