/** square.c
 *
 *  A simple program to move the Create in a square 1m per side.
 *
 *  Author: Jesse DeGuire
 *
 * 
 *  This file is part of COIL.
 *
 *  COIL is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  COIL is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 * 
 *  You should have received a copy of the GNU General Public License
 *  along with COIL.  If not, see <http://www.gnu.org/licenses/>.
 * 
 *
 *  Versions:
 *	1.0 	12 Jan 2008	Initial public release
 */

#include <createoi.h>
#include <stdio.h>
#include <iostream>
#include <math.h>

// 回転誤差
#define P_TURN_A100 1.1002386
#define P_TURN_B100 2.247255

#define N_TURN_A100 1.1002386
#define N_TURN_B100 (-2.247255)

#define VELOCITY -100.0
#define RADIUS 1		//-1:右回転 1:左回転
#define RAD_MAX 10


int main()
{

	int angle=0;
	int total_angle = 0;
	float hosei_angle=0.0;
	startOI_MT("/dev/ttyUSB0");
	
	printf("start_d:%d\n",getDistance());
	printf("start_a:%d\n",getAngle());

	for(int i=0;i<1;i++)
	{
	printf("start_d:%d\n",getDistance());
	printf("start_a:%d\n",getAngle());

		angle=0;
		drive (VELOCITY, RADIUS);
		while(1){

			angle += getAngle();

			if(fabs(angle) >= RAD_MAX)//RAD_MAX)
			{
				drive (0, 0);
				total_angle += angle;
				hosei_angle +=P_TURN_A100*angle + P_TURN_B100;
				fprintf(stdout, "正回転角度:%f\n", P_TURN_A100*angle + P_TURN_B100);
				fprintf(stdout, "負回転角度:%f\n", N_TURN_A100*angle + N_TURN_B100+angle);
				fprintf(stdout, "回転角度:%d\n", angle);
				fprintf(stdout, "総回転角度:%f\n", hosei_angle);
				break;
			}
		}

	//	drive (0, 0);
	printf("start_d:%d\n",getDistance());
	printf("start_a:%d\n",getAngle());

		waitTime(1.0);
	printf("start_d:%d\n",getDistance());
	printf("start_a:%d\n\n",getAngle());


	}

	stopOI_MT();

	return 0;
}
