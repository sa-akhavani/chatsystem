CC=gcc

all: server client

server: tracker.o
	$(CC) tracker.o -o server
tracker.o: tracker.c tracker.h
	$(CC) -c tracker.c

client: client.o
	$(CC) client.o -o client
client.o: client.c client.h
	$(CC) -c client.c

clean:
	rm *.o server client
