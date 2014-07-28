# This is a very simple Makefile for compiling COIL sample
# applications. 


CC = g++ -g
INCLUDE = /usr/local/include
LIBS = -lm -lncurses -lcreateoi -lpthread
CVFLAGS = $(shell pkg-config --libs opencv)  $(shell pkg-config --cflags opencv)

default: all


all: drive sonerSensor getCoordinate setMesh

drive: drive.cpp
	$(CC) drive.cpp $(LIBS) -o drive

sonerSensor: sonerSensor.cpp
	$(CC) sonerSensor.cpp $(LIBS) -o sonerSensor

getCoordinate: getCoordinate.cpp
	$(CC) getCoordinate.cpp $(LIBS) -o getCoordinate

setMesh: setMesh.cpp
	$(CC) setMesh.cpp $(LIBS) -o setMesh


clean:
	rm -f *.o
	rm -f drive
