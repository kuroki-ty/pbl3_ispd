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
		this->tracks.map_point_list.push_back(this->create.present_coord);
		this->wall_search_flag = true;

		this->create.Straight_Run = false;
		this->create.Right_Run    = false;
		this->create.Left_Run     = false;
	}


void checkState();


bool getWallSearchFlag()
{
	return wall_search_flag;
}

void calculateCreateCoordinate();

void recordCoordinate( Coordinate coord; )
{
	this->tracks.recordCoordinate( coord );	// world座標に記録
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
