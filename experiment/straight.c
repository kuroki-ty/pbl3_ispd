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
int main()
{
	startOI_MT ("/dev/ttyUSB0");
	float distance=0,angle=0;
	float tmp_x,tmp_y,current_angle;
	//distance= getDistance();
	//angle= getAngle();
	drive(200,0);
	while(1)
	{
		distance += getDistance();
		angle += getAngle();
		
		if(distance >= 1000)
		{
			current_angle = angle * (M_PI / 180);
			tmp_x = distance * cos(current_angle);
			tmp_y = distance * sin(current_angle);
			printf("x座標: %f ,y座標: %f,",tmp_x,tmp_y);
			break;
		}
	}

	stopOI_MT();
}
