CC=g++
CFLAGS=-fopenmp

all: index

index: ABC.o Node.o index.o Batoh.o
	$(CC) $(CFLAGS) ABC.o Node.o index.o Batoh.o -o a.out

ABC.o:	ABC.cpp
	$(CC) $(CFLAGS) -c ABC.cpp

Node.o:  Node.cpp
	$(CC) $(CFLAGS) -c Node.cpp

index.o: index.cpp
	$(CC) $(CFLAGS) -c index.cpp

Batoh.o: Batoh.cpp
	$(CC) $(CFLAGS) -c Batoh.cpp

clean:
	rm -rf *.o

