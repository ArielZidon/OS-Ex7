.PHONY: all run clean
CC = gcc -fPIC -g -w
OBJECTS = file.o Mylibc.o Tests.o  
FLAGS= 
HEADERS = 
all: main TEST lib.so

main: main.o  file.o  Mylibc.o 
	$(CC) main.o  Mylibc.o file.o -o main

TEST: Tests.o file.o  Mylibc.o 
	$(CC) Tests.o  Mylibc.o file.o -o TEST

lib.so: $(OBJECTS)
	$(CC) --shared -fPIC -g -pthread $(OBJECTS) -o lib.so

%.o: %.c 
	$(CC) -c $< -o $@

clean:
	rm -f *.o main TEST lib.so

