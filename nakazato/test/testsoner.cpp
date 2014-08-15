#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include<iterator>
#include <createoi.h>
float getDistanceBySoner()
{
	int analog_input = 0;
	
	analog_input = readSensor(SENSOR_ANALOG_SIGNAL);	//アナログ値取得
	
	return ((6450.0/1023.0)*(float)analog_input);		//アナログ値[v]→距離[mm]		
}

int main()
{
	startOI_MT("/dev/ttyUSB0");

while(1)
{
	std::cout << "soner====" << getDistanceBySoner() <<  std::endl;
}
	stopOI_MT();	
	return 0;
}
