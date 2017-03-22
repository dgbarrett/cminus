#ifndef __ARGS_H__
#define __ARGS_H__

#include <string.h>

#define MAX_ARGS 5

extern int AST_FLAG;
extern int SYMBTABLE_FLAG;

int isCMinusFile(const char * fname);
int isFlag(const char * flag);
void setFlag(const char * flag);

#endif

