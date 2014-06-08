#include <createoi.h>

#define VELOCITY 200		//直進速度[mm/s]
#define RADIUS_LEFT 230		//左回り回転半径[mm]
#define RADIUS_RIGHT 320	//右回り回転半径[mm]
#define WALL_DISTANCE_LOW 15.0	//壁とCreateの距離_低[cm]
#define WALL_DISTANCE_HIGH 20.0	//壁とCreateの距離_高[cm]

#define CREATE_SIZE 340 // 縦 [mm]
// 横330[mm]

// soner
#define SONEROFFSET_X 		150 // [mm]
#define SONEROFFSET_ANGLE	45 						 //degree
#define SONEROFFSET_ANGLE_L	(180-SONEROFFSET_ANGLE_R) //degree
// bumper
#define BUMPER_PLACE_OFFSET 170
#define BUMPER_PLACE_ANGLE_R  45		//[deg]
#define BUMPER_PLACE_ANGLE_L  (180-BUMPER_PLACE_ANGLE_R)



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

enum Bumper
{
	NONE,
	RIGHT,
	LEFT,
	CENTER
};



class Create
{
public:
	Create()
	{
		this->present_direction = PLUS_X;
		this->distance=0.0;
		this->velocity=0.0; 
		this->total_angle=0.0;
		this->push_bumper = NONE;
	}

// public method

	// 超音波センサを使って直進
	void goStraightWithSoner();

	// とにかく走らせる
	void run();

	void stopRun()
	{
		drive(0,0);
	}

	// 障害物に衝突したときの方向転換
	void changeDirection();



// set method
	void setDirection(enum Direction d)
	{
		this->present_direction = d;
	}

	// Createの角度を加える
	void addAngle(int angle)
	{
		this->total_angle += angle;
	}


	// 現在座標の値を更新
	void updatePresentCoordinateXY(float X, float Y)
	{
		this->present_coord.setX(X);
		this->present_coord.setY(Y);
	}


// get method
	int getTotalAngle()	
	{
		return this->total_angle;
	}

	int getAngleFromCreate()
	{
		return getAngle() % 360;
	}

	Coordinate getPresentCoordinate()
	{
		return this->present_coord;
	}



//public 変数
	bool Straight_Run;
	bool Right_Run;
	bool Left_Run;
	enum State state;
	enum Bumper push_bumper;

private:
	int distance;
	int velocity;
	int total_angle;

	enum Direction present_direction;

	Coordinate present_coord;


};

#include "Create.cpp"
