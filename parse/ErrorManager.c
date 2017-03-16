#include "ErrorManager.h"

/* Manages all parser errors and related functions */

Error * new_Error(char * message, int line, int yy) {
	Error * err = malloc(sizeof(*err));

	err -> message = calloc(strlen(message)+1, sizeof(*(err->message)));
	strcpy(err->message, message);

	err -> line = line;
	err -> isyy = yy;

	return err;
}

void Error_print(Error * err) {
	if (err) {
		
		if (err -> message) {
			if (err -> isyy == 1) {
				fprintf(stderr, "         [line %3d] %s\n", err->line, err->message);
			} else {
				fprintf(stderr, "[ ERROR ][line %3d] %s\n", err->line, err->message);
			}
		}
	}
}

/* Holds list of errors in the program */
ErrorList * new_ErrorList() {
	ErrorList * list = malloc(sizeof(*list));

	list -> head = NULL;
	list -> tail = NULL;
	list -> size = 1;

	return list;
}
int ErrorList_isEmpty(ErrorList * list) {
	return list && list -> size == 0;
}

/* Insert a new error into the error list */
void ErrorList_insert(ErrorList * list, Error * err) {
	if (list -> head == NULL) {
		list -> head = err;
		list -> tail = err;
	} else {
		Error * temp = list -> head;
		while (temp) {
			if (temp -> line >= err -> line) {
				if (temp -> line == err -> line) {
					temp -> message = err-> message;
					temp -> isyy = err -> isyy;
				} else {
					if (list -> head == temp) {
						list -> head -> prev = err;
						list -> head = err;
						list -> head -> next = temp;
					} else {
						err -> prev = temp -> prev;
						err -> next = temp;
						temp -> prev = err;
						err -> prev -> next = err;
					}
					list -> size++;	
				}
				return;			
			}
			temp = temp -> next;
		}

		list -> tail -> next = err;
		err -> prev = list -> tail;
		list -> tail = err;
		list -> size++;
	}

}

/* Print all errors (in order of line number) */
void ErrorList_print(ErrorList * err) {
	Error * temp = err -> head;

	while(temp) {
		Error_print(temp);

		temp = temp -> next;
	}
}