#include <createoi.h>

#define VELOCITY 200		//直進速度[mm/s]
#define RADIUS_LEFT 230		//左回り回転半径[mm]
#define RADIUS_RIGHT 320	//右回り回転半径[mm]
#define WALL_DISTANCE_LOW 15.0	//壁とCreateの距離_低[cm]
#define WALL_DISTANCE_HIGH 20.0	//壁とCreateの距離_高[cm]


enum Direction
{
	PLUS_X,
	MINUS_X,
	PLUS_Y,
	MINUS_Y
};

enum State
{
    STOP,
    RUN,
    TURN,
	BUMPER
};



class Create
{
public:
	Create()
	{
		this->present_direction = PLUS_X;
		this->distance=0.0;
		this->velocity=0.0; 
		this->angle=0.0;

	}

// public method

// 超音波センサを使って直進
void goStraightWithSoner();

// とにかく走らせる
void run();

// 障害物に衝突したときの方向転換
void changeDirection();

// 超音波センサが当たるポイントの座標（world座標）
Coordinate sonerHitPoint();

// set method
void setDirection(enum Direction d)
{
	this->present_direction = d;
}

// get method
int getCreateAngle()
{
	return this->angle;
}

void addAngle(int angle)
{
	this->angle += angle;
}

//public 変数
bool Straight_Run;
bool Right_Run;
bool Left_Run;

private:
int distance;
int velocity;
int angle;

enum Direction present_direction;
Coordinate present_coord;


};

#include "Create.cpp"
