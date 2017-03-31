#ifndef __DMEM_SYMBOL_H__
#define __DMEM_SYMBOL_H__

typedef enum _AddrType {
	ABSOLUTE,
	FP_RELATIVE
} AddrType;

typedef struct _DMemSymbol {
	char * name;
	char * type;
	int arrSize;
	int dMemAddr;
	AddrType addressType;
} DMemSymbol;

DMemSymbol * new_DMemSymbol(char * name, char * type, int arrSize, int dMemAddr, AddrType addressType);
void DMemSymbol_print(DMemSymbol * symbol);

#endif