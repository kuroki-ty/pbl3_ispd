#include"Tracks.hpp"
#include"Create.hpp"

#define PI 3.141592


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
Coordinate calculateSonerHitPointCoordinate();
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


// 宣言
	Tracks tracks;	// Createの蓄えた軌跡情報（ブロックと座標系）
	Create create;

private:

	enum State state;
	enum State last_state; 

	bool wall_search_flag;


};

#include "Controller.cpp"
