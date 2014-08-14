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
	Coordinate next_point(500,0);
	create.setTotalAngle(0);
	bool Bumper = false;

	std::vector<Coordinate> create_list; 
	std::vector<Coordinate> obstacle_list; 	

	std::vector<Coordinate> tmp_obstacle_list; 	

	startOI_MT("/dev/ttyUSB0");
	
	create.showInfo();
	create.runNextPoint(next_point, Bumper, create_list, obstacle_list);
	if(Bumper)
	{
		std::vector<Coordinate> create_coord;
		Bumper = false;
		tmp_obstacle_list = create.searchObstacle(create_coord);
		for(int i=0;i<tmp_obstacle_list.size();i++)
		{
			obstacle_list.push_back(tmp_obstacle_list[i]);
		}
		for(int i=0;i<create_coord.size();i++)
		{
			create_list.push_back(create_coord[i]);
		}
	}
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

	std::ofstream ofs("/home/qito-asmd-2/output/search_obstacle_test/obstacle_list");
	for(int i=0;i<obstacle_list.size();i++)
	{
		ofs << i << "\t" << obstacle_list[i].x << "\t" << obstacle_list[i].y << std::endl;
	}
	ofs.close();

	std::ofstream ofsc("/home/qito-asmd-2/output/search_obstacle_test/create_list");
	for(int i=0;i<create_list.size();i++)
	{
		ofsc << i << "\t" << create_list[i].x << "\t" << create_list[i].y << std::endl;
	}
	ofsc.close();


	return 0;
}
