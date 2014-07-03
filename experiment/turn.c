//回転誤差実験用コード

#include <createoi.h>
#include <stdio.h>

#define VEL 200
#define RAD -1
#define LIM_RAD 90

int main()
{
	int a=0;
	startOI_MT("/dev/ttyUSB0");
	
	drive (VEL, RAD);
	while(1){
		a += getAngle();
		if(a >= 90) break;
	}

	fprintf(stdout, "回転角度:%d\n", a);

	stopOI_MT();

	return 0;
}
