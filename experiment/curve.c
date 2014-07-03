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
	DISTANCE_MAXに入れるときは四捨五入してint型で．
*/

#include <createoi.h>
#include <stdio.h>
#include <math.h>

#define VELOCITY 		200
#define RADIUS 			200
#define DISTANCE_MAX 	52 

int main()
{
	startOI_MT ("/dev/ttyUSB0");

	int distance=0,angle=0;
	float x, y;
	drive(VELOCITY, RADIUS);
	while(1)
	{
		distance += getDistance();
		angle += getAngle();
		
		if(distance >= DISTANCE_MAX)
		{
			angle_r = (float)(distance / RADIUS); //[rad]
			
			x = 2 * RADIUS * sin(angle_r/2) * cos(angle_r/2);
			y = 2 * RADIUS * sin(angle_r/2) * sin(angle_r/2);
			
			fprintf(stdout, "x座標: %f , y座標: %f , 回転角度: %d\n", x, y, angle);
			break;
		}
	}

	stopOI_MT();
}
