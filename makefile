.PHONY: all run clean
CC = gcc
FLAGS= 
HEADERS = 
all: main 

main: main.o file.o  Mylibc.o
	$(CC) main.o file.o -o main

Mylibc.o: Mylibc.c Mylibc.h
	$(CC) -g -c Mylibc.c

%.o: %.c 
	$(CC) -c $< -o $@

clean:
	rm -f *.o main