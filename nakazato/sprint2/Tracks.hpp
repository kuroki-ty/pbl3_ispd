#include <iostream>
#include <vector>
#include "Coordinate.hpp"
#include "Block.hpp"

#define MAX_COORD_SIZE 29.4 //[m] block lenghが0.35[m]なので合計84ブロックになる

class Tracks
{
public:
	// コンストラクタ 座標とブロック数を初期化

	// 地図のブロックの初期化
	void blockListInit();
	
	//
	void recordCoordinate(Coordinate coord)
	{
		this->map_point_list.push_back(coord);
	}

private:
//  Coordinate max;
	std::vector< std::vector<Block> > block_list;	// 探索用メッシュ
	std::vector<Coordinate> map_point_list;			// マップ生成用座標リスト
//  std::vector< Block > wall_list;
//  Block* current_block;
// Block* next_block;
//  std::queue<Block *> qu_path;
// MapGraph mapgraph;
//  double angle_to_next;
//  double distance_to_next;
//  std::queue<Block *> qu_obstacle;
//  Block* next_obstacle;
//  int flg_border;

};

#include "Tracks.cpp"
