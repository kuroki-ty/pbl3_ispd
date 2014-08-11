# This is a very simple Makefile for compiling COIL sample
# applications. 


CC = gcc -g -Wall
INCLUDE = /usr/local/include
LIBS = -lm -lncurses -lcreateoi -lpthread
CVFLAGS = $(shell pkg-config --libs opencv)  $(shell pkg-config --cflags opencv)

default: all

all: C_a C_b C_c

C_a: Compulsory_a.c
	$(CC) Compulsory_a.c $(LIBS) -o C_a

C_b: Compulsory_b.c
	$(CC) Compulsory_b.c $(LIBS) -o C_b

C_c: Compulsory_c.c
	$(CC) Compulsory_c.c $(LIBS) -o C_c

clean:
	rm -f *.o
	rm -f C_a C_b C_c
