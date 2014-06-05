#include"Tracks.hpp"
#include"Create.hpp"


class Controller
{
public:
	Controller()
	{
		this->tracks.blockListInit();
		this->create.setDirection(PLUS_X);
		this->wall_search_flag = true;
	}


void checkBumper();

bool getWallSearchFlag()
{
	return wall_search_flag;
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
