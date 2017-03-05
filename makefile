CC = gcc
CFLAGS = -Wall
OBJS = y.tab.o lex.yy.o main.o tokens.o SyntaxTree.o

all : cminus

.PHONY : cminus
cminus : 
	yacc -d cminus.y
	gcc -c y.tab.c
	flex cminus.l 
	gcc -c lex.yy.c 
	gcc -c tokens.c
	gcc -c SyntaxTree.c
	gcc -c main.c
	gcc y.tab.o lex.yy.o tokens.o SyntaxTree.o main.o -o cminus -ll -ly

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
	rm -f lex.yy.*
	rm -f y.tab.*
	rm -f *.o
	rm -f cminus