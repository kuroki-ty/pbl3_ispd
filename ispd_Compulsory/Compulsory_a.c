#include <createoi.h>
#include <stdio.h>

int main()
{
	startOI_MT("/dev/ttyUSB0");

	int distance;

	drive(200, 0);	//直進動作
	distance = getDistance();
	printf("走行距離: %d¥n", distance);

	//5000[mm]走るまで直進
	while(1){
		printf("走行距離: %d¥n", distance);
		if(distance > 4500){
			drive(100, 0);
		}
		if(distance > 5000){
			drive(0, 0);
			break;
		}
	}

	stopOI_MT();

	return 0;
}
