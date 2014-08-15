/*カーブ誤差実験用コード

実験内容

速度変化 : ±100~±300(50刻みで計測)
回転半径変化 : 200~300(50刻みで計測)
距離変化 : 15度,30度
↑距離計算法 : r*θ = d(角度を基に走行距離を計算すること)

計測観点
距離(x, y)誤差(設定値と実測値)
一応，回転角度も調査する

	r	*	θ	=	d
	200		π/12	52.36
	250		π/12	65.45
	300		π/12	78.54
	200		π/6		104.72
	250		π/6		130.90
	300		π/6		157.08

	230		π/6		120.43
	230		π/3		240.86
	230		π/2		361.28
		
	-320	π/6		167.55	
	-320	π/3		335.10
	-320	π/2		502.65

	
	DISTANCE_MAXに入れるときは四捨五入してint型で．
*/

#include <createoi.h>
#include <stdio.h>
#include <math.h>

#define VELOCITY 		200.0
#define RADIUS 			-320.0
#define DISTANCE_MAX 	502.65

int main()
{
	startOI_MT ("/dev/ttyUSB0");

	int distance=0,angle=0;
	float x, y, angle_r;

	printf("start_d:%d\n",getDistance());
	printf("start_a:%d\n",getAngle());

	drive(VELOCITY, RADIUS);
	while(1)
	{
		distance += getDistance();
		angle += getAngle();
		
		if(distance >= DISTANCE_MAX)
		{
			angle_r = (float)(distance / RADIUS); //[rad]
			
			x = 2.0 * RADIUS * sin(angle_r/2.0) * cos(angle_r/2.0);
			y = 2.0 * RADIUS * sin(angle_r/2.0) * sin(angle_r/2.0);
			
			fprintf(stdout, "x座標: %f , y座標: %f , 回転角度: %d\n", x, y, angle);
			printf("angle_r:%f\n",angle_r*(180/M_PI));
			break;
		}
	}

	stopOI_MT();
}
