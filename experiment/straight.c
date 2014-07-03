/*直進誤差実験用コード

実験内容

速度変化 ±100--±300　50刻み
距離変化　300--900　300刻み

計測観点
縦・横誤差（設定値と実測値）

*/

#include <createoi.h>
#include <stdio.h>
#include <math.h>

#define VELOCITY 		200
#define DISTANCE_MAX 	300

int main()
{
	startOI_MT ("/dev/ttyUSB0");

	float distance=0,angle=0;
	float x,y,current_angle;

	drive(VELOCITY, 0);
	while(1)
	{
		distance += getDistance();
		angle += getAngle();
		
		if(distance >= DISTANCE_MAX)
		{
			current_angle = angle * (M_PI / 180);
			x = distance * cos(current_angle);
			y = distance * sin(current_angle);

			fprintf(stdout, "x座標: %f , y座標: %f\n", x, y);

			break;
		}
	}

	stopOI_MT();
}
