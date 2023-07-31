
MWST : MWST.o
	gcc -o MWST MWST.o
MWST.o : MWST.c
	gcc -c -std=c99 -Wall MWST.c
clean :
	rm -f MWST MWST.o
