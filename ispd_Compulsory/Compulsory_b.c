#include <createoi.h>
#include <stdio.h>

#define ANGLE_ERROR 10

int main()
{
	startOI_MT("/dev/ttyUSB0");

	int angle = 0;

	drive(0, 0);

	/***1. 反時計回り180度***/
	printf("反時計回り180度\n");
	angle = getAngle();
	drive(200, 1);
	printf("現在角度: %d\n", angle);

	while(1){
		if(angle >= (150-ANGLE_ERROR)){
			break;
		}
		angle += getAngle();
	}
	printf("現在角度: %d\n", angle);

	printf("/***減速***/\n");
	drive(50, 1);
	while(1){
		if(angle >= (180-ANGLE_ERROR)){
			break;
		}
		angle += getAngle();
	}
	printf("現在角度: %d\n", angle);
	/*********************/

	//3秒間静止
	printf("3秒間静止\n");
	drive(0, 0);
	angle = getAngle();
	waitTime(3.0);
	angle = 0;

	/***2. 時計回り180度***/
	printf("時計回り180度\n");
	angle = getAngle();
	drive(200, -1);
	printf("現在角度: %d\n", angle);

	while(1){
		if(angle <= -(150-ANGLE_ERROR)){
			break;
		}
		angle += getAngle();
	}
	printf("現在角度: %d\n", angle);

	printf("/***減速***/\n");
	drive(50, -1);
	while(1){
		if(angle <= -(180-ANGLE_ERROR)){
			break;
		}
		angle += getAngle();
	}
	printf("現在角度: %d\n", angle);
	/*********************/

	//3秒間静止
	printf("3秒間静止\n");
	drive(0, 0);
	angle = getAngle();
	waitTime(3.0);
	angle = 0;

	/***3. 反時計回り180度***/
	printf("反時計回り180度\n");
	angle = getAngle();
	drive(200, 1);
	printf("現在角度: %d\n", angle);

	while(1){
		if(angle >= (150-ANGLE_ERROR)){
			break;
		}
		angle += getAngle();
	}
	printf("現在角度: %d\n", angle);

	printf("/***減速***/\n");
	drive(50, 1);
	while(1){
		if(angle >= (180-ANGLE_ERROR)){
			break;
		}
		angle += getAngle();
	}
	printf("現在角度: %d\n", angle);
	/*********************/

	//3秒間静止
	printf("3秒間静止\n");
	drive(0, 0);
	angle = getAngle();
	waitTime(3.0);
	angle = 0;

	/***4. 時計回り180度***/
	printf("時計回り180度\n");
	angle = getAngle();
	drive(200, -1);
	printf("現在角度: %d\n", angle);

	while(1){
		if(angle <= -(150-ANGLE_ERROR)){
			break;
		}
		angle += getAngle();
	}
	printf("現在角度: %d\n", angle);

	printf("/***減速***/\n");
	drive(50, -1);
	while(1){
		if(angle <= -(180-ANGLE_ERROR)){
			break;
		}
		angle += getAngle();
	}
	printf("現在角度: %d\n", angle);
	/*********************/

	//3秒間静止
	printf("3秒間静止\n");
	drive(0, 0);
	angle = getAngle();
	waitTime(3.0);
	angle = 0;

	/***5. 反時計回り180度***/
	printf("反時計回り180度\n");
	angle = getAngle();
	drive(200, 1);
	printf("現在角度: %d\n", angle);

	while(1){
		if(angle >= (150-ANGLE_ERROR)){
			break;
		}
		angle += getAngle();
	}
	printf("現在角度: %d\n", angle);

	printf("/***減速***/\n");
	drive(50, 1);
	while(1){
		if(angle >= (180-ANGLE_ERROR)){
			break;
		}
		angle += getAngle();
	}
	printf("現在角度: %d\n", angle);
	/*********************/
	
	drive(0, 0);

	stopOI_MT();

	return 0;
}
