#include "gen.h"

#include <stdio.h>
#include <stdlib.h>

/* TMFile */
typedef struct _TMFile {
	int pc, sp;
	FILE fp;
} TMFile;

TMFile * new_TMFile(char * filename) {
	TMFile * tm = malloc(sizeof(*tm));

	tm -> pc = 0;
	tm -> sp = 0;

	tm -> fp = fopen(filename, "w");

	return tm;
}


char * getTmFilename(char * fname);

void generateCode(ASTNode * root, char * fname) {
	char * tmFilename = getTmFilename(fname);
	TMFile * tmFile = new_TMFile(fname);

	if (TMFile) {

	}
}

char * getTmFilename(char * fname) {
	int i = 0;

	for ( i = 0 ; fname[i] != '\0' && i < NAME_MAX; i++) {
		if (fname[i] == '.') break;
	}

	fname[i+1] = 't';
	fname[i+2] = 'm';
	fname[i+3] = '\0';

	return fname;
}