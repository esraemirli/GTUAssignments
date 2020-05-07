all: xxx

xxx: programA.o programB.o
	gcc programA.o -o programA
	gcc programB.o -o programB -lm

programA.o: programA.c
	gcc -c programA.c

programB.o: programB.c
	gcc -c programB.c

clean:
	rm *.o programA programB
