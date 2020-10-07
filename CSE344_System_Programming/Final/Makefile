all: xxx

xxx: server.o client.o
	gcc server.o -o server -pthread
	gcc client.o -o client 

server.o: server.c
	gcc -c server.c

client.o: client.c
	gcc -c client.c

clean:
	rm *.o client server
