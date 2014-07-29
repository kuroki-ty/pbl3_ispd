# This is a very simple Makefile for compiling COIL sample
# applications. 


CC = gcc -g -Wall
INCLUDE = /usr/local/include
LIBS = -lm -lncurses -lcreateoi -lpthread
CVFLAGS = $(shell pkg-config --libs opencv)  $(shell pkg-config --cflags opencv)

default: all


all: square drive tracker wander

ir_test: ir_test.c
	$(CC) ir_test.c $(LIBS) -o ir_test

square: square.c
	$(CC) square.c $(LIBS) -o square

drive: drive.c
	$(CC) drive.c $(LIBS) -o drive

wander: wander.c
	$(CC) wander.c $(LIBS) $(CVFLAGS) -o wander
	
tracker: tracker.c
	$(CC) tracker.c $(LIBS) $(CVFLAGS) -o tracker

clean:
	rm -f *.o
	rm -f square drive wander tracker
