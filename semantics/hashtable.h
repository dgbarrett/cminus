#ifndef __SYMBOL_HASH_TABLE_H__
#define __SYMBOL_HASH_TABLE_H__

#include "symbol.h"

typedef struct _SymbolHashTable {
	int size;
	int maxsize;
	Symbol ** symbols;
} SymbolHashTable;

/* SymbolHashTable */
SymbolHashTable * 	new_SymbolHashTable		();
int 				HashTable_hash			(char * name);
int 				HashTable_insert		(SymbolHashTable * st, Symbol * symbol);
Symbol * 			HashTable_get			(SymbolHashTable * st, char * name);
Symbol ** 			HashTable_getSymbols	(SymbolHashTable * ht);

#endif