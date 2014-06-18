#include <iostream>
#include <vector>
#include "Coordinate.hpp"

#define MAX_COORD_SIZE 29.4 //[m] block lenghが0.35[m]なので合計84ブロックになる

class Tracks
{
public:
	// コンストラクタ 座標とブロック数を初期化
	

// set method
	// createの座標リストをプッシュバック
	void push_back_CreatePointList(Coordinate coord)
	{
		this->create_point_list.push_back( coord );
	}

	// 障害物の座標リストをプッシュバック
	void push_back_ObstaclePointList( Coordinate coord )
	{
		this->obstacle_point_list.push_back(coord);
	}

// get method
	float getCurrentCreatePoint_X()
	{
		Coordinate coord;
		std::vector<Coordinate>::iterator iterator;
		iterator = this->create_point_list.end();
		coord = *iterator;
		return coord.getX();
	}
	float getCurrentCreatePoint_Y()
	{
		Coordinate coord;
		std::vector<Coordinate>::iterator iterator;
		iterator = this->create_point_list.end();
		coord = *iterator;
		return coord.getY();
	}

	// create listの配列数を取得
	int getCreateListSize()
	{
		return this->create_point_list.size();
	}
	// obstacle list の配列数を取得
	int getObstacleListSize()
	{
		return this->obstacle_point_list.size();
	}

std::vector<Coordinate> getCreatePointList()
{
	return this->create_point_list;
}

std::vector<Coordinate> getObstaclePointList()
{
	return this->obstacle_point_list;
}


private:
//  Coordinate max;
	std::vector<Coordinate> create_point_list;			// create座標リスト
	std::vector<Coordinate> obstacle_point_list;	// （マップ生成用）障害物座標リスト
};

