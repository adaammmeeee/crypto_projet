run: compil
	./main

debug: compil
	valgrind --leak-check=full ./main


lib.o: lib.c lib.h
	gcc -Wall -c -g lib.c -lgmp

main.o: main.c lib.h
	gcc -Wall -c -g main.c -lgmp

compil: lib.o main.o
	gcc -Wall -o main main.o lib.o -lgmp
