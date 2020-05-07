all: xxx

xxx: program.o program.o
	gcc program.o -o program -lm

program.o: program.c
	gcc -c program.c -lm

clean:
	rm *.o program