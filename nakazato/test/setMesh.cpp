#include "Coordinate.h"
#include "./block/Block.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include<iterator>

int main()
{
	Coordinate tmp_create_coord;	
	std::vector<Coordinate> create_coord;
	Coordinate tmp_obstacle_coord;	
	std::vector<Coordinate> obstacle_coord;

	Block block;

	// create_coord読み込み
    std::ostringstream create_file_path;
	create_file_path << "./test_create_obstacle_coord/create_list.txt";
    std::ifstream create_file(create_file_path.str().c_str());
	std::vector<float> create(std::istream_iterator<float>(create_file), (std::istream_iterator<float>()));

	// obstacle_coord読み込み
    std::ostringstream obstacle_file_path;
	obstacle_file_path << "./test_create_obstacle_coord/obstacle_list.txt";
    std::ifstream obstacle_file(obstacle_file_path.str().c_str());
	std::vector<float> obstacle(std::istream_iterator<float>(obstacle_file), (std::istream_iterator<float>()));

	// Coordinate構造体に書き換え
	for (unsigned i = 0; i < create.size()/3; i++)
	{
		tmp_create_coord.x = create[3*i+1];
		tmp_create_coord.y = create[3*i+2];
		create_coord.push_back(tmp_create_coord);
	}

	// Coordinate構造体に書き換え
	for (unsigned i = 0; i < obstacle.size()/3; i++)
	{
		tmp_obstacle_coord.x = obstacle[3*i+1];
		tmp_obstacle_coord.y = obstacle[3*i+2];
		obstacle_coord.push_back(tmp_obstacle_coord);
	}
	// create_coordの表示
	//for (unsigned i = 0; i < create_coord.size(); i++)
	//{
	//	std::cout << i << "  " << create_coord[i].x <<", "<< create_coord[i].y << std::endl;
	//}


	// create_coordの登録（setMesh）
	for (unsigned i = 0; i < create_coord.size(); i++)
	{
		block.setMeshMark( create_coord[i], false );
		block.showMesh();
	}
	// create_coordの登録（setMesh）
	for (unsigned i = 0; i < obstacle_coord.size(); i++)
	{
		block.setMeshMark( obstacle_coord[i], true );
		block.showMesh();
	}
	block.fillMesh();

	return 0;
}
