/*超音波センサの値を取得するプログラム*/

#include <createoi.h>
#include <stdlib.h>
#include <stdio.h>
#include <curses.h>
#include <math.h>

#define VELOCITY 200		//直進速度[mm/s]
#define RADIUS_LEFT 230		//左回り回転半径[mm]
#define RADIUS_RIGHT 320	//右回り回転半径[mm]
#define WALL_DISTANCE_LOW 15.0	//壁とCreateの距離_低[cm]
#define WALL_DISTANCE_HIGH 20.0	//壁とCreateの距離_高[cm]


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
		drive(VELOCITY, 0);	//壁との距離が範囲内であれば直進
	}
	else if(getDistanceBySoner() < WALL_DISTANCE_LOW)
	{
		drive(VELOCITY, RADIUS_LEFT);	//壁と近い場合、左向きに直進
	}
	else
	{
		drive(VELOCITY, -RADIUS_RIGHT);	//壁と遠い場合、右向きに直進
	}
}

int main()
{
	startOI("/dev/ttyUSB0");

	while(getBumpsAndWheelDrops() != 2)
	{		
		traveling();
	}

	stopOI();

	return 0;
}
