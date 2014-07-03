# This is a very simple Makefile for compiling COIL sample
# applications. 


CC = gcc -g
INCLUDE = /usr/local/include
LIBS = -lm -lncurses -lcreateoi -lpthread
CVFLAGS = $(shell pkg-config --libs opencv)  $(shell pkg-config --cflags opencv)

default: all


all: straight turn curve

straight: straight.c
	$(CC) straight.c $(LIBS) -o straight

turn: turn.c
	$(CC) turn.c $(LIBS) -o turn

curve: curve.c
	$(CC) curve.c $(LIBS) -o curve
clean:
	rm -f *.o
