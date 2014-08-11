#include <createoi.h>
#include <stdio.h>

int main()
{
	startOI_MT("/dev/ttyUSB0");

	int distance;
	int angle;

	//2000[mm]走るまで若干右カーブ, 速度200で直進
	directDrive(200, 203);	//直進動作
	distance = getDistance();
	angle = getAngle();
	printf("走行距離: %d\n", distance);
	printf("現在角度: %d\n", angle);

	while(1){
		if(distance >= 2000){
			break;
		}
		distance += getDistance();
		angle += getAngle();
	}

	//4500[mm]走るまで若干左カーブ, 速度200で直進
	directDrive(210, 200);	//直進動作
	printf("走行距離: %d\n", distance);
	printf("現在角度: %d\n", angle);

	while(1){
		if(distance >= 4500){
			break;
		}
		distance += getDistance();
		angle += getAngle();
	}

	//5000[mm]走るまで若干右カーブ, 速度100で直進
	//若干カーブしている分、5000では短い
	directDrive(90, 100);
	printf("走行距離: %d\n", distance);
	while(1){
		distance += getDistance();
		angle += getAngle();
		if(distance >= 5095){
			printf("走行距離: %d\n", distance);
			printf("現在角度: %d\n", angle);
			drive(0, 0);
			break;
		}
	}

	stopOI_MT();

	return 0;
}
