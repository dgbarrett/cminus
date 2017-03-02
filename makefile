CC = gcc
CFLAGS = -Wall
OBJS = lex.yy.o main.o tokens.o

all : cminus

cminus: $(OBJS)
	$(CC) $(CFLAGS) $^ -o cminus -ll

lex.yy.o: cminus.l tokens.h lex.h
	flex cminus.l
	$(CC) $(CFLAGS) -c lex.yy.c

main.o: main.c lex.h
	$(CC) $(CFLAGS) -c $<

tokens.o : tokens.c tokens.h
	$(CC) $(CFLAGES) -c $<

clean:
	rm -f lex.yy.*
	rm -f *.o
	rm -f cminus