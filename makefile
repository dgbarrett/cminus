CC = gcc
CFLAGS = -Wall
OBJS = cminus.tab.o cminus.yy.o main.o tokens.o ast.o ErrorManager.o

all : cm

.PHONY : cm
cm : 
	yacc -o cminus.tab.c -d parse/cminus.y
	gcc -c cminus.tab.c
	flex -o cminus.yy.c scan/cminus.l 
	gcc -c cminus.yy.c 
	gcc -c scan/tokens.c
	gcc -c parse/ast.c
	gcc -c parse/ErrorManager.c
	gcc -c parse/ast_print.c
	gcc -c semantics/SymbolTable.c
	gcc -c args.c
	gcc -c main.c
	gcc cminus.tab.o cminus.yy.o tokens.o ast.o main.o ErrorManager.o ast_print.o args.o SymbolTable.o -o cm -ll -ly

clean:
	rm -f *.yy.*
	rm -f *.tab.*
	rm -f *.o
	rm -f cm