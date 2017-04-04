#include "args.h" 

int AST_FLAG = 0;
int SYMBTABLE_FLAG = 0;
int ASM_FLAG = 0;

/* Checks if a given filename represents a c- file */
int isCMinusFile(const char * fname) {
	if (fname) {
		int len = strlen(fname) + 1, i;
		if (len > 4) {
			for (i=len ; i >= 0 ; i--) {
				if (fname[i] == '.') {
					return fname[i+1] == 'c' && fname[i+2] == 'm' && fname[i+3] == '\0';
				}
			}
		} 
	}
	return 0;
}

/* Checks if a given string represent a commmand line flag */
int isFlag(const char * flag) {
	return strcmp(flag, "-a") == 0 ||
		   strcmp(flag, "-s") == 0 ||
		   strcmp(flag, "-c") == 0;
}

/* Sets a flag */
void setFlag(const char * flag) {
	if (strcmp(flag, "-a") == 0) {
		AST_FLAG = 1;
	} else if (strcmp(flag, "-s") == 0) {
		SYMBTABLE_FLAG = 1;
	} else if (strcmp(flag, "-c") == 0) {
		ASM_FLAG = 1;
	}
}