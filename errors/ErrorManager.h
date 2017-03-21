#ifndef __ERROR_MANAGER_H__
#define __ERROR_MANAGER_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct _Error {
	int line, isyy;
	char * message;
	struct _Error * prev, *next;
} Error;

typedef struct _ErrorList {
	int size;
	Error * head;
	Error * tail;
} ErrorList;

Error * new_Error(char * message, int line, int yy) ;
ErrorList * new_ErrorList() ;
int ErrorList_isEmpty(ErrorList * list);
void ErrorList_insert(ErrorList * list, Error * err) ;
void ErrorList_print(ErrorList * err) ;

#endif