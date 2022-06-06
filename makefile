.PHONY: all run clean
CC = gcc
FLAGS= 
HEADERS = 
all: main

main: main.o file.o
	$(CC) main.o file.o -o main

%.o: %.c 
	$(CC) -c $< -o $@

clean:
	rm -f *.o main