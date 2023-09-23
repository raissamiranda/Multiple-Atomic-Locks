CC = gcc
CFLAGS = -Wall -g -c -lm -pthread -I .

all: threads

threads: threads.o spend_time.o
	$(CC) -Wall -g threads.o spend_time.o -o threads -lm -pthread

threads.o: threads.c
	$(CC) threads.c $(CFLAGS)

spend_time.o: spend_time.c
	$(CC) spend_time.c $(CFLAGS)