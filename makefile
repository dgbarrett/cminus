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
	gcc -c -Wall scan/tokens.c
	gcc -c -Wall errors/errtemplates.c
	gcc -c -Wall errors/ErrorManager.c
	gcc -c -Wall parse/ast.c
	gcc -c -Wall parse/ast_print.c
	gcc -c -Wall semantics/symbol.c
	gcc -c -Wall semantics/hashtable.c
	gcc -c -Wall semantics/symbtable.c
	gcc -c -Wall semantics/scope.c
	gcc -c -Wall semantics/symbtable_print.c
	gcc -c -Wall args.c
	gcc -c -Wall main.c
	gcc -c -Wall semantics/checking.c
	gcc -c -Wall gen/gen.c
	gcc cminus.tab.o cminus.yy.o tokens.o ast.o main.o ErrorManager.o ast_print.o args.o symbtable.o symbtable_print.o checking.o hashtable.o symbol.o scope.o errtemplates.o gen.o -o cm -ll -ly

clean:
	rm -f *.yy.*
	rm -f *.tab.*
	rm -f *.o
	rm -f cm