CC = g++
CFLAGS = -lpthread -g

all: fibo

fibo: bib.o fibo.cpp
	$(CC) -o fibo.o bib.o fibo.cpp $(CFLAGS)
fatorial: bib.o fatorial.cpp
	$(CC) -o fatorial.o bib.o fatorial.cpp $(CFLAGS)
bib.o: main.cpp header.hpp
	$(CC) -o bib.o main.cpp -c $(CFLAGS)

clean:
	rm -rf *.o *~ 

