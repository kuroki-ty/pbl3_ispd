/*回転誤差実験用コード

実験内容

実験1
速度変化　±50--±200　50刻みで計測
角度変化　90-180　90刻みで計測

実験2
15 30 45
速度50固定で計測

計測観点
角度誤差（設定値と実測値）

*/

#include <createoi.h>
#include <stdio.h>

#define VELOCITY -100.0
#define RADIUS 1		//-1:右回転 1:左回転
#define RAD_MAX -4


int main()
{
	int angle=0;
	float angle2=0;
	startOI_MT("/dev/ttyUSB0");
	
	printf("start_d:%d\n",getDistance());
	printf("start_a:%d\n",getAngle());

	drive (VELOCITY, RADIUS);
	while(1){

		angle += getAngle();

		if(angle <= RAD_MAX)//RAD_MAX)
		{
			fprintf(stdout, "回転角度:%f\n", 0.111*angle - 14.543+angle);
			fprintf(stdout, "回転角度:%d\n", angle);
			break;
		}
	}

	stopOI_MT();

	return 0;
}
