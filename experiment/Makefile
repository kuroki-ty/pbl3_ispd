# This is a very simple Makefile for compiling COIL sample
# applications. 


CC = gcc -g
INCLUDE = /usr/local/include
LIBS = -lm -lncurses -lcreateoi -lpthread
CVFLAGS = $(shell pkg-config --libs opencv)  $(shell pkg-config --cflags opencv)

default: all


all: straight turn curve

straight: straight.cpp
	$(CC) straight.cpp $(LIBS) -o straight

turn: turn.cpp
	$(CC) turn.cpp $(LIBS) -o turn

curve: curve.cpp
	$(CC) curve.cpp $(LIBS) -o curve
clean:
	rm -f *.o
