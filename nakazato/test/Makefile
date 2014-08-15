# This is a very simple Makefile for compiling COIL sample
# applications. 


CC = g++ -g
INCLUDE = /usr/local/include
LIBS = -lm -lncurses -lcreateoi -lpthread
#CVFLAGS = $(shell pkg-config --libs opencv)  $(shell pkg-config --cflags opencv)

# OpenCV のライブラリを指定
OPENCVINC = `pkg-config --cflags opencv`
OPENCVLIB = `pkg-config --libs opencv`


default: all


all: drive sonerSensor getCoordinate setMesh cluster testBumper turnturn kmeans serchObstacle runNextPoint testsoner testturn

drive: drive.cpp
	$(CC) drive.cpp $(LIBS) -o drive

sonerSensor: sonerSensor.cpp
	$(CC) sonerSensor.cpp $(LIBS) -o sonerSensor

getCoordinate: getCoordinate.cpp
	$(CC) getCoordinate.cpp $(LIBS) -o getCoordinate

setMesh: setMesh.cpp
	$(CC) setMesh.cpp $(LIBS) -o setMesh

cluster: cluster.cpp
	$(CC) cluster.cpp $(LIBS) -o cluster

testBumper: testBumper.cpp
	$(CC) testBumper.cpp $(LIBS) -o testBumper

turnturn: turnturn.cpp
	$(CC) turnturn.cpp $(LIBS) -o turnturn

kmeans: kmeans.cpp
	$(CC) kmeans.cpp $(LIBS) $(OPENCVLIB) -o kmeans

serchObstacle: serchObstacle.cpp
	$(CC) serchObstacle.cpp $(LIBS) $(OPENCVLIB) -o serchObstacle

runNextPoint: runNextPoint.cpp
	$(CC) runNextPoint.cpp $(LIBS) -o runNextPoint

testsoner: testsoner.cpp
	$(CC) testsoner.cpp $(LIBS) -o testsoner

testturn: testturn.cpp
	$(CC) testturn.cpp $(LIBS) -o testturn


clean:
	rm -f *.o
	rm -f drive
