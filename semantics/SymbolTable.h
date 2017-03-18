#ifndef __SYMBOL_TABLE_H__
#define __SYMBOL_TABLE_H__

typedef struct _Symbol {
	char * name;
	int isFunction;
} Symbol;

typedef enum _ScopeType {
	SELECTION_PART,
	WHILE_LOOP,
	FUNCTION,
	FILE
} ScopeType;

typedef struct _Scope {
	ScopeType type;
	Symbol ** symbols;
	struct Scope ** subscopes;
} Scope;

typedef struct _SymbolTable {
	Scope * root;
	int size;
} SymbolTable;	

#endif