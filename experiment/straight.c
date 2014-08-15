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

#define VELOCITY 	-100.0
#define DISTANCE_MAX 	-50

int main()
{
	startOI_MT ("/dev/ttyUSB0");

	int distance=0,angle=0;
	float x,y,current_angle;
	printf("start_d:%d\n",getDistance());
	printf("start_a:%d\n",getAngle());


	drive(VELOCITY, 0);
	while(1)
	{
		distance += getDistance();
		angle += getAngle();
		//printf("%d\n",distance);

		if(distance <= DISTANCE_MAX)
		{
			drive(0, 0);
			current_angle = (float)(angle * (M_PI / 180));
			x = (float)distance * cos(current_angle);
			y = (float)distance * sin(current_angle);

			fprintf(stdout, "x座標: %f , y座標: %f\n", x, y);
			x = -0.0122*x+67.354+x;			
			y = -0.0242*x;
			fprintf(stdout, "補正x座標: %f , 補正y座標: %f\n", x, y);
			printf("angle:%d  :  distance:%d\n",angle,distance);
			waitTime(1.0);
			break;
		}
	}
	stopOI_MT();

}
