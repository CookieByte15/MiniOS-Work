
CC = gcc
CFLAGS = -Wall -pthread -Iinclude

.PHONY: all clean

all: queue_test semaphore_demo producer_consumer race_demo

queue_test: examples/queue_test.c src/sync.c include/minios.h
	$(CC) $(CFLAGS) examples/queue_test.c src/sync.c -o queue_test

semaphore_demo: examples/semaphore_demo.c
	$(CC) $(CFLAGS) examples/semaphore_demo.c -o semaphore_demo

producer_consumer: examples/producer_consumer.c
	$(CC) $(CFLAGS) examples/producer_consumer.c -o producer_consumer

race_demo: examples/race_demo.c
	$(CC) $(CFLAGS) examples/race_demo.c -o race_demo

clean:
	rm -f queue_test semaphore_demo producer_consumer race_demo sync.o

