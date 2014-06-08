#include <createoi.h>

#define VELOCITY 200			//直進速度[mm/s]
#define RADIUS_LEFT  230		//左回り回転半径[mm]
#define RADIUS_RIGHT 320		//右回り回転半径[mm]
#define WALL_DISTANCE_LOW  150.0	//壁とCreateの距離_低[cm]
#define WALL_DISTANCE_HIGH 200.0	//壁とCreateの距離_高[cm]

#define CREATE_SIZE 340 // 縦 [mm]
// 横330[mm]

// soner
#define SONEROFFSET_X 		150 // [mm]
#define SONEROFFSET_ANGLE	(45 * (M_PI / 180)) 			//rad
#define SONEROFFSET_ANGLE_L	((180-SONEROFFSET_ANGLE_R) * (M_PI / 180))	//[rad]
// bumper
#define BUMPER_PLACE_OFFSET 170 //[mm]
#define BUMPER_PLACE_ANGLE_R  (45 * (M_PI / 180))		//[rad]
#define BUMPER_PLACE_ANGLE_L  ((180-BUMPER_PLACE_ANGLE_R) * (M_PI / 180))



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
		this->distance=0;
		this->velocity=0; 
		this->total_angle=0;
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
	// 超音波センサから距離値を得る[mm]	
	float getDistanceBySoner();



// set method
	void setDirection(enum Direction d)
	{
		this->present_direction = d;
	}

	// Createの角度を加える
	void addAngle(int angle)
	{
		this->total_angle += angle;
		this->total_angle = this->total_angle % 360;
	}

	void addDistance(int distance)
	{
		this->distance +=distance;
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

	int getDistanceFromCreate()
	{
		return getDistance();
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

	int distance;

private:
	int velocity;
	int total_angle;

	enum Direction present_direction;

	Coordinate present_coord;


};

#include "Create.cpp"
