# This is a very simple Makefile for compiling COIL sample
# applications. 


CC = g++ -g
INCLUDE = /usr/local/include
LIBS = -lm -lncurses -lcreateoi -lpthread
CVFLAGS = $(shell pkg-config --libs opencv)  $(shell pkg-config --cflags opencv)

default: all


all: main

main: main.cpp
	$(CC) main.cpp $(LIBS) -o main

clean:
	rm -f *.o
	rm -f drive
