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
//#define P_TURN_A100 1.1002386
//#define P_TURN_B100 2.247255

//#define N_TURN_A100 1.1002386
//#define N_TURN_B100 (-2.247255)

#define VELOCITY -100.0
#define RADIUS 1		//-1:右回転 1:左回転
#define RAD_MAX 180

// 回転誤差補正値
#define P_TURN_A100 (-0.0003)
#define P_TURN_B100 0.1227
#define P_TURN_C100 2.6985

#define N_TURN_A100 (0.0002)
#define N_TURN_B100 0.12
#define N_TURN_C100 (-3.3513)


int main()
{

	int angle=0;
	int total_angle = 0;
	float hosei_angle=0.0;
	float huhosei_angle=0.0;
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
				//waitTime(1.0);
				total_angle += angle;
				hosei_angle +=P_TURN_A100*angle*angle + P_TURN_B100*angle + P_TURN_C100 + angle;
				huhosei_angle +=N_TURN_A100*angle*angle + N_TURN_B100*angle + N_TURN_C100 + angle;
				std::cout << "正回転角度==========" <<  P_TURN_A100*angle*angle + P_TURN_B100*angle + P_TURN_C100 + angle << std::endl;
				std::cout << "負回転角度==========" <<  N_TURN_A100*angle*angle + N_TURN_B100*angle + N_TURN_C100 + angle << std::endl;
				std::cout << "補正無し回転角度=====" <<  angle << std::endl;
				std::cout << "正総回転角度==========" <<  hosei_angle << std::endl;
				std::cout << "負総回転角度==========" <<  huhosei_angle << std::endl;
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
