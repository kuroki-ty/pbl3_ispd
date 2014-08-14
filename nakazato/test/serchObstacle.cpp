#include "Coordinate.h"
#include "./block/Block.h"
#include "./block/Map.h"

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
	Map map;

	// create_coord読み込み
    std::ostringstream create_file_path;
//	create_file_path << "./test_create_obstacle_coord/create_list.txt";
	create_file_path << "/home/qito-asmd-2/output/create_list.txt";
    std::ifstream create_file(create_file_path.str().c_str());
	std::vector<float> create(std::istream_iterator<float>(create_file), (std::istream_iterator<float>()));

	// obstacle_coord読み込み
    std::ostringstream obstacle_file_path;
//	obstacle_file_path << "./test/obstacle_list.txt";
	obstacle_file_path << "/home/qito-asmd-2/output/wall_list.txt";
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
	//for (unsigned i = 0; i < obstacle_coord.size(); i++)
	//{
	//	std::cout << i << "  " << obstacle_coord[i].x <<", "<< obstacle_coord[i].y << std::endl;
	//}


	// create_coordの登録（setMesh）
	for (unsigned i = 0; i < create_coord.size(); i++)
	{
		block.setMeshMark( create_coord[i], false );
		block.showMesh();
	}
	std::vector<Coordinate> p;		//壁直線の交点を格納する配列
    std::vector<bool> xflag;		//壁直線がy=ax+bかy=cかを判定する変数
    std::vector< std::vector<float> > ransac;	//壁直線の係数を格納する2次元配列 ransac[i][0]:a, ransac[i][1]:b ransac[i][2]:c
	map.setWallPointList(obstacle_coord); //Mapクラスのobstacleリストに渡す

	/*壁のメッシュを埋める処理 yamasaki記述*/
	map.calcLine();	//Mapクラスのobstacleリストから直線式を計算する
	p = map.getIntersectionLine();	//直線の交点をgetする
    ransac = map.getCoefficientLine(xflag);	//直線式の係数をgetする
	map.showMap2();

    
    block.setMeshMarks(p, ransac, xflag);		//壁のメッシュをまとめて埋める
	block.fillMesh();							//壁のメッシュの外側を全て埋める
	block.showMesh();

// 障害物探索ここから

		int goal;
		std::vector<Coordinate> move_point_list;  // Createが辿る座標を格納したリスト

		//create_coord = this->create.getCurrentCoordinate();
		while(1) // メッシュが全部埋まるまで
		{
		
			goal = block.getNextMeshNum();
			move_point_list.clear();
			// 移動座標のリスト取得 create_coord[create_coord.size()-1]を壁探索終了直後の現在座標とした
			move_point_list = block.getMovePointList(create_coord[create_coord.size()-1], MINUS_Y, goal);

std::cout << "move_list : "<< move_point_list.size() << std::endl;

// 移動リストの全座標出力
for(int i=0;i<move_point_list.size();i++)
{
	std::cout << "move_list("<< move_point_list[i].x << "," << move_point_list[i].y <<") "<< std::endl;
}
			for(int i=0;i<move_point_list.size();i++)
			{
				block.setMeshMark(move_point_list[i], false);// 移動先リストにぴったり移動したものとして、メッシュを埋める
				map.push_back_CreatePointList( move_point_list[i] );
				create_coord.push_back(move_point_list[i]);	// 現在座標の更新
				block.showMesh();
			}
			if( block.checkAllSearchEnd() )
			{
std::cout << "FINISHED!" << std::endl;
				break;

			}
		}



	

	return 0;
}
