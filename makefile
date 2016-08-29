all: p5a p5b
p5a: clean maina.o bst.o
	gcc maina.o bst.o -o p5a
maina.o: maina.c bst.h proto.h const.h
	gcc -c maina.c
bst.o: bst.c bst.h
	gcc -c bst.c
p5b: clean mainb.o archive.o
	gcc mainb.o archive.o -o p5b
mainb.o: mainb.c bst.h proto.h const.h
	gcc -c mainb.c
archive.o: archive.c const.h
	gcc -c archive.c
clean: 
	rm -f *.o core
