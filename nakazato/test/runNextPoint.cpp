#include <createoi.h>
#include "Coordinate.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include<iterator>

#include <stdio.h>
#include <math.h>
#include "./block/Create.h"



int main()
{
	Create create;
	Coordinate next_point(10,10);
	create.setTotalAngle(315);
	bool Bumper = false;

	std::vector<Coordinate> create_list; 
	std::vector<Coordinate> obstacle_list; 	

	startOI_MT("/dev/ttyUSB0");
	
	create.showInfo();
	create.runNextPoint(next_point, Bumper, create_list, obstacle_list);
	create.showInfo();

	for (unsigned i = 0; i < create_list.size(); i++)
	{
		std::cout << i << "  " << create_list[i].x <<", "<< create_list[i].y << std::endl;
	}
	for (unsigned i = 0; i < obstacle_list.size(); i++)
	{
		std::cout << i << "  " << obstacle_list[i].x <<", "<< obstacle_list[i].y << std::endl;
	}

	stopOI_MT();

	return 0;
}
