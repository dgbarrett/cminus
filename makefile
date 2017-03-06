CC = gcc
CFLAGS = -Wall
OBJS = cminus.tab.o cminus.yy.o main.o tokens.o ast.o ErrorManager.o

all : cminus

.PHONY : cminus
cminus : 
	yacc -o cminus.tab.c -d parse/cminus.y
	gcc -c cminus.tab.c
	flex -o cminus.yy.c scan/cminus.l 
	gcc -c cminus.yy.c 
	gcc -c scan/tokens.c
	gcc -c parse/ast.c
	gcc -c parse/ErrorManager.c
	gcc -c main.c
	gcc -c parse/error.c 
	gcc cminus.tab.o cminus.yy.o tokens.o ast.o main.o error.o ErrorManager.o -o cminus -ll -ly

build:  $(OBJS)
	$(CC) $(CFLAGS) $^ -o cminus -ll -ly

main.o: y.tab.o lex.yy.o main.c lex.h
	$(CC) $(CFLAGS) -c $<

tokens.o : tokens.c tokens.h
	$(CC) $(CFLAGES) -c $<

SyntaxTree.o : SyntaxTree.c SyntaxTree.h
	$(CC) $(CFLAGES) -c $<

scan : cminus.l cminus.y
	flex cminus.l
	yacc -d cminus.y
	$(CC) $(CFLAGS) -c y.tab.c
	$(CC) $(CFLAGS) -c lex.yy.c

clean:
	rm -f *.yy.*
	rm -f *.tab.*
	rm -f *.o
	rm -f cminus