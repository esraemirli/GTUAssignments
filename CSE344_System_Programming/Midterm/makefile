all: xxx

xxx: program.o program.o
	gcc program.o -o program -pthread

program.o: program.c 
	gcc -c program.c 

clean:
	rm *.o program
