CC=gcc

all: server client

server: tracker.o linkedlist.o
	$(CC) tracker.o linkedlist.o -o server
tracker.o: tracker.c tracker.h
	$(CC) -c tracker.c
linkedlist.o: linkedlist.c linkedlist.h
	$(CC) -c linkedlist.c


client: client.o
	$(CC) client.o -o client
client.o: client.c client.h
	$(CC) -c client.c

clean:
	rm *.o server client
