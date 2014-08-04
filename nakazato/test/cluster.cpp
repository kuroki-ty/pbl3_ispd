
#include "Coordinate.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include<iterator>
#include<stdio.h>
#include<stdlib.h>

#include<time.h>/* 追加部分 */

int main()
{

	srand((unsigned)time(NULL));/* 追加部分 */

	Coordinate tmp_create_coord;	
	std::vector<Coordinate> create_coord;
	Coordinate tmp_obstacle_coord;	
	std::vector<Coordinate> obstacle_coord;

	float ran;
	std::vector<Coordinate> cluster(3);
	cluster[0].x = 150;
	cluster[0].y = 100;

	cluster[1].x = 600;
	cluster[1].y = 300;

	cluster[2].x = 200;
	cluster[2].y = 700;


	while(obstacle_coord.size() < 800)
	{

	while(1)
	{
		ran = rand() %1000;
		if( ran < cluster[0].x +150 && ran > cluster[0].x -150  )
		{
			tmp_obstacle_coord.x = ran;
			break;
		}
	}
	while(1)
	{
		ran = rand() %1000;
		if( ran < cluster[0].y +130 && ran > cluster[0].y -140  )
		{
			tmp_obstacle_coord.y = ran;
			break;
		}
	}
	obstacle_coord.push_back( tmp_obstacle_coord );

	while(1)
	{
		ran = rand() %1000;
		if( ran < cluster[1].x +80 && ran > cluster[1].x -80  )
		{
			tmp_obstacle_coord.x = ran;
			break;
		}
	}
	while(1)
	{
		ran = rand() %1000;
		if( ran < cluster[1].y +80 && ran > cluster[1].y -80  )
		{
			tmp_obstacle_coord.y = ran;
			break;
		}
	}
	obstacle_coord.push_back( tmp_obstacle_coord );


	while(1)
	{
		ran = rand() %1000;
		if( ran < cluster[2].x +100 && ran > cluster[2].x -120  )
		{
			tmp_obstacle_coord.x = ran;
			break;
		}
	}
	while(1)
	{
		ran = rand() %1000;
		if( ran < cluster[2].y +100 && ran > cluster[2].y -100  )
		{
			tmp_obstacle_coord.y = ran;
			break;
		}
	}
	obstacle_coord.push_back( tmp_obstacle_coord );

	}

	while(obstacle_coord.size() < 850)
	{

		tmp_obstacle_coord.x = rand()%1000;
		tmp_obstacle_coord.y = rand()%1000;
		obstacle_coord.push_back(tmp_obstacle_coord);

	}

    std::ostringstream cluster_file_path;
	cluster_file_path << "./cluster_list.txt";
    std::ofstream ofs(cluster_file_path.str().c_str());

	for(int i=0;i<obstacle_coord.size();i++)
	{
   		 // ファイルに1行ずつ書き込み
	    std::cout << i << '\t' << obstacle_coord[i].x << '\t' << obstacle_coord[i].y << std::endl;

	}


	// 出力
	for(int i=0;i<obstacle_coord.size();i++)
	{
   		 // ファイルに1行ずつ書き込み
	    ofs << i << '\t' << obstacle_coord[i].x << '\t' << obstacle_coord[i].y << std::endl;

	}

	return 0;
}
