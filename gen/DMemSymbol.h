#ifndef __DMEM_SYMBOL_H__
#define __DMEM_SYMBOL_H__

typedef struct _DMemSymbol {
	char * name;
	char * type;
	int arrSize;
	int dMemAddr;
} DMemSymbol;

DMemSymbol * new_DMemSymbol(char * name, char * type, int arrSize, int dMemAddr);
void DMemSymbol_print(DMemSymbol * symbol);

#endif