.PHONY: all run clean
CC = gcc
FLAGS= 
HEADERS = 
all: main TEST 

main: main.o file.o  Mylibc.o 
	$(CC) main.o  Mylibc.o file.o -o main

TEST: Tests.o file.o  Mylibc.o 
	$(CC) Tests.o  Mylibc.o file.o -o TEST

lib.so: file.o 
	$(CC) --shared -fPIC -g file.o  -o lib.so

%.o: %.c 
	$(CC) -c $< -o $@

clean:
	rm -f *.o main TEST