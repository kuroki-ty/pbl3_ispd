#include <createoi.h>
#include <stdio.h>

int main()
{
	startOI_MT("/dev/ttyUSB0");

	runDemo(DEMO_COVER_AND_DOCK);		//ドッキングステーションに入るためのDemo

	int i;

	for(i=0; i<10; i++){
		printf("%d\n", readSensor(SENSOR_INFRARED));	//ドッキングステーションからのIR値を読み込んでくる
		waitTime(1);
	}

	stopOI_MT();

	return 0;
}
