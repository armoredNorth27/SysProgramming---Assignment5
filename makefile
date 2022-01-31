main: client.o server.o tic-tac-toe.o
	gcc -o client client.o
	gcc -o server server.o tic-tac-toe.o

client.o: client.h client.c
	gcc -c client.c

server.o: server.h server.c
	gcc -c server.c

tic-tac-toe.o: tic-tac-toe.h tic-tac-toe.c
	gcc -c tic-tac-toe.c

clean:
	rm -f *.o
	rm -f client
	rm -f server