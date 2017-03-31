#include "DMemSymbol.h"

#include <stdlib.h>
#include <stdio.h>

#include "Instruction.h"

DMemSymbol * new_DMemSymbol(char * name, char * type, int arrSize, int dMemAddr, AddrType addressType) {
	DMemSymbol * symbol = malloc(sizeof(*symbol));

	symbol -> name = new_Name(name);
	symbol -> type = new_Name(type);
	symbol -> arrSize = arrSize;
	symbol -> dMemAddr = dMemAddr;
	symbol -> addressType = addressType;

	return symbol;
}

void DMemSymbol_print(DMemSymbol * symbol) {
	if (symbol) {
		if (symbol -> arrSize) {
			printf("%s %s[%d]\n", symbol -> type, symbol -> name, symbol -> arrSize);
		} else {
			printf("%s %s\n", symbol -> type, symbol -> name);
		}
	}
}
/**/