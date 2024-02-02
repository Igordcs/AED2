CC=gcc
CFLAGS=-I.

all: programa

programa: bst.o avl.o arb.o sgbd.o main.o
	$(CC) -o programa bst/bst.o avl/avl.o arb/arb.o sgbd.o main.o

bst.o: bst/bst.c
	$(CC) -c bst/bst.c

avl.o: avl/avl.c
	$(CC) -c avl/avl.c

arb.o: arb/arb.c
	$(CC) -c arb/arb.c

sgbd.o: sgbd.c
	$(CC) -c sgbd.c

main.o: main.c
	$(CC) -c main.c

clean:
	rm -f bst/*.o avl/*.o arb/*.o *.o programa
