/*超音波センサの値を取得するプログラム*/

#include <createoi.h>
#include <stdlib.h>
#include <stdio.h>
#include <curses.h>
#include <math.h>
#include <iostream>

#define VELOCITY 150		//直進速度[mm/s]
#define RADIUS_LEFT 230		//左回り回転半径[mm]
#define RADIUS_RIGHT 320	//右回り回転半径[mm]
#define WALL_DISTANCE_LOW 15.0	//壁とCreateの距離_低[cm]
#define WALL_DISTANCE_HIGH 20.0	//壁とCreateの距離_高[cm]

int totalangle=0;
int totaldist = 0;
int totaldist_cos = 0;

//AnalogInputに接続した超音波センサから距離を取得するメソッド
double getDistanceBySoner()
{
	int analog_input = 0;
	
	analog_input = readSensor(SENSOR_ANALOG_SIGNAL);	//アナログ値取得
	
	return ((645.0/1023.0)*(double)analog_input);		//アナログ値[v]→距離[cm]	
	
}

//走行メソッド
void traveling(){
	if((getDistanceBySoner() >= WALL_DISTANCE_LOW) && (getDistanceBySoner() <= WALL_DISTANCE_HIGH))
	{
		//std::cout << "1：" << getAngle() << std::endl;
		drive(VELOCITY, 0);	//壁との距離が範囲内であれば直進
		//std::cout << "2:" << getAngle() << std::endl;	
	}
	else if(getDistanceBySoner() < WALL_DISTANCE_LOW)
	{
		//std::cout << "1左:" << getDistance() << std::endl;
		drive(VELOCITY, RADIUS_LEFT);	//壁と近い場合、左向きに直進
		//std::cout << "2左:" << getDistance() << std::endl;	
	}
	else
	{
		//std::cout << "1右:" << getAngle() << std::endl;
		drive(VELOCITY, -RADIUS_RIGHT);	//壁と遠い場合、右向きに直進
		//std::cout << "2右:" << getAngle() << std::endl;	
	}
}

int main()
{
	startOI("/dev/ttyUSB0");
	int a=0;
driveDistance(-100,0,9-100,1);
	while(getBumpsAndWheelDrops() == 0)
	{


/*
std::cout << "上" << getAngle() << "|" << cos(getAngle()) << "|" << getDistance() <<  std::endl;
		traveling();
		totalangle +=getAngle();
		totaldist += getDistance();
		totaldist_cos += getDistance() * cos(getAngle());
std::cout << "下" << getAngle() << "|" << cos(getAngle()) << "|" << getDistance() <<  std::endl;
*/

		
		a++;
		 std::cout << "a:"<< a << std::endl;
	}


	std::cout << "total angle:" << totalangle << std::endl;
	std::cout << "total dist cos" << totaldist_cos << "total dist: " << totaldist << std::endl;

	stopOI();

	return 0;
}
