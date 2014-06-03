# This is a very simple Makefile for compiling COIL sample
# applications. 


CC = g++ -g
INCLUDE = /usr/local/include
LIBS = -lm -lncurses -lcreateoi -lpthread
CVFLAGS = $(shell pkg-config --libs opencv)  $(shell pkg-config --cflags opencv)

default: all


all: drive

drive: drive.cpp
	$(CC) drive.cpp $(LIBS) -o drive

clean:
	rm -f *.o
	rm -f drive
