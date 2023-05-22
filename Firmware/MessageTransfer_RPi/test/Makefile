CXX=g++
CC=gcc
INCDIR=../include
CFLAGS=-Wall -I$(INCDIR) -g


all: stest pqtest

stest: socket_test.cpp
	$(CXX) $(CFLAGS) -o $@ $^ -lpthread

priority_queue.o: ../src/priority_queue.c 
	$(CC) $(CFLAGS) -c -o $@ $^

pqtest: priority_queue_test.c priority_queue.o
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm stest pqtest