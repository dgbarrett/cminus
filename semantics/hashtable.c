#include "hashtable.h"

#include <stdlib.h>

#include "symbol.h"

#define HASH_TABLE_SIZE 499

/*************** SymbolHashTable ***************/

/*
	Function: new_SymbolHashTable
		Creates a new hash table to hold symbols.
*/
SymbolHashTable * new_SymbolHashTable() {
	SymbolHashTable * st = malloc(sizeof(*st));

	st -> symbols = calloc(HASH_TABLE_SIZE, sizeof(*(st->symbols)));
	st -> maxsize = HASH_TABLE_SIZE;
	st -> size = 0;

	return st;
}

/*
	Function: HashTable_insert
		Insert a new symbol into a SymbolHashTable. Returns 1 on success, 0 if 
		the symbol already exists or some other error occured.
*/
int HashTable_insert(SymbolHashTable * st, Symbol * symbol) {
	if (st && symbol) {
		int hash = HashTable_hash(symbol -> name);
		if (st -> symbols[hash] != NULL) return 0;
		else {
			st -> symbols[hash] = symbol;
			st -> size++;
			return 1;
		}
	} else return 0;
}

/*
	Function: HashTable_get
		Get the symbol with name from a SymbolHashTable. Returns NULL if no
		such symbol exists in the hash table.
*/
Symbol * HashTable_get(SymbolHashTable * st, char * name) {
	if (st && name) {
		int hash = HashTable_hash(name);
		return st -> symbols[hash];
	}
	return NULL;
}

/*
	Function: HashTable_hash
		Hash a name to an integer for Symbol storage or retrival in a 
		SymbolHashTable.
*/
int HashTable_hash(char * name) {
	if (name) {
		int i = 0, temp = 0;
		for (i = 0 ; name[i] != '\0' ; i++) {
			temp = ((temp << 4) + (name[i] * (i+222))) % HASH_TABLE_SIZE;
		}	
		return temp;
	} else return -1;
}

/*
	Function: HashTable_getSymbols
		Get an array of the symbols contained in the hash table.
*/
Symbol ** HashTable_getSymbols(SymbolHashTable * ht) {
	Symbol ** arr = calloc(ht -> size, sizeof(*arr));
	int i = 0 ,index=0;
	for (i=0 ; i<ht->maxsize ; i++ ) {
		if (ht -> symbols[i] != NULL) {
			arr[index++] = ht -> symbols[i];
		}
	}

	return arr;
}