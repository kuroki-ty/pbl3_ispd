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

int main()
{

	int d=0,a=0,v=0,t=0;
	startOI_MT("/dev/ttyUSB0");

	d = getDistance();
	a = getAngle();
	//v = getVelocity();
	t = getTurningRadius();
	
	d = 0;
	a = 0;
	t = 0;

	drive (200, 0);	
	while(1){


		d += getDistance();
		a += getAngle();
		//v = getVelocity();
		t = getTurningRadius();
std::cout << d << std::endl;
		if(d >= 1000) 
		{
			printf("a %d\n",d);	
			break;
		}
	}

//	fprintf(stdout, "距離:%d, 回転角度:%d, カーブ半径:%d\n", d,a,t);
	std::cout << "距離：" << d << "回転角度:" << a << "カーブ半径：" << t <<  std::endl;

	stopOI_MT();

	return 0;
}
