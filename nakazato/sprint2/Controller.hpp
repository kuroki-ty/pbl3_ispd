#include"Tracks.hpp"
#include"Create.hpp"
#include <fstream>

#define RECORD_OBSTACLE_TH 300

class Controller
{
public:
	Controller()
	{
		this->tracks.blockListInit();
		this->create.setDirection(PLUS_X);
		this->tracks.push_back_MapPointList(this->create.getPresentCoordinate());
		this->wall_search_flag = true;

		this->create.Straight_Run = false;
		this->create.Right_Run    = false;
		this->create.Left_Run     = false;
	}


// Createの現在座標（world）を計算
Coordinate calculateCreateCoordinate();
Coordinate calculateCreateCoordinate(int distance, int angle);
// 超音波センサが当たる部分の座標（world）の計算
Coordinate calculateSonerHitPointCoordinate(float soner_distance);
Coordinate calculateBumperHitPointCoordinate();

void checkState();


bool getWallSearchFlag()
{
	return wall_search_flag;
}

// 最大絶対値を360にしてCreateから角度を取得
int getAngleFromCreate()
{
	return getAngle() % 360;
}


// Createの座標を記録
void recordCreateCoordinate( Coordinate coord )
{
	this->tracks.push_back_MapPointList( coord );	// world座標に記録
}

void recordObstacleCoordinate( Coordinate coord )
{
	this->tracks.push_back_ObstaclePointList( coord );
}

void output_MapList()
{
	std::vector<Coordinate> map_list;
	map_list = this->tracks.getMapPointList();

	std::ofstream ofs("map_list");
	for(int i=0;i<this->tracks.getMapListSize();i++)
	{
		ofs << i << "\t" << map_list[i].getX() << "\t" << map_list[i].getY() << std::endl;
	}
	ofs.close();
}

void output_ObstacleList()
{
	std::vector<Coordinate> obstacle_list;
	obstacle_list = this->tracks.getObstaclePointList();

	std::ofstream ofs("obstacle_list");
	for(int i=0;i<this->tracks.getMapListSize();i++)
	{
		ofs << i << "\t" << obstacle_list[i].getX() << "\t" << obstacle_list[i].getY() << std::endl;
	}
	ofs.close();
}

// 宣言
	Tracks tracks;	// Createの蓄えた軌跡情報（ブロックと座標系）
	Create create;

private:

	enum State state;
	enum State last_state; 

	bool wall_search_flag;
};

#include "Controller.cpp"
