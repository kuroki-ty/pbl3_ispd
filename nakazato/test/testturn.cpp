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
#include <math.h>
#include <iostream>


#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include<iterator>

#define VELOCITY 100.0
#define RADIUS 1		//-1:右回転 1:左回転
#define RAD_MAX 4

// 回転誤差補正値
#define P_TURN_A100 (-0.0003)
#define P_TURN_B100 0.1227
#define P_TURN_C100 2.6985

#define N_TURN_A100 (0.0002)
#define N_TURN_B100 0.12
#define N_TURN_C100 (-3.3513)

float turnCreate(int velocity, int angle_max)
{
	int angle=0;

std::cout << "distance & angle init" << std::endl;
std::cout << getDistance() << std::endl;
std::cout << getAngle() << std::endl;

	waitTime(1.0);
	drive(velocity, 1);
	while(1)
	{
		angle += getAngle();
		if(fabs(angle) >= fabs(angle_max))
		{
			drive(0,0);
			break;
		}
	}
	waitTime(0.2);
	if(angle >0 )
	{
			std::cout <<"turn------------------------:" << P_TURN_A100 * angle * angle + P_TURN_B100 * angle + P_TURN_C100+ angle << std::endl;
			return P_TURN_A100 * angle * angle + P_TURN_B100 * angle + P_TURN_C100 + angle;
	}
	else if(angle == 0)
	{
			std::cout <<"turn------------------------:" << P_TURN_A100 * angle << std::endl;
			return P_TURN_A100 * angle;
	}
	else
	{
			std::cout <<"turn------------------------:" << N_TURN_A100*angle + N_TURN_B100 << std::endl;
			return N_TURN_A100 * angle * angle + N_TURN_B100 * angle + N_TURN_C100 + angle;
	}
}

int main()
{
float turn_angle;
	startOI_MT("/dev/ttyUSB0");

for(int i=0;i<5;i++)
{
	turn_angle = turnCreate(VELOCITY,RAD_MAX);

	std::cout << turn_angle << std::endl;
}
	stopOI_MT();

	return 0;
}
