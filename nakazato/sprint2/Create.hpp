#include <createoi.h>


/*****************************************************************************
 ** Define
 *****************************************************************************/
#define VELOCITY 200			//直進速度[mm/s]
#define RADIUS_LEFT  230.0		//左回り回転半径[mm]
#define RADIUS_RIGHT 320.0		//右回り回転半径[mm]
#define WALL_DISTANCE_LOW  150.0	//壁とCreateの距離_低[cm]
#define WALL_DISTANCE_HIGH 200.0	//壁とCreateの距離_高[cm]

#define CREATE_SIZE 340 // 縦 [mm]
// 横330[mm]

// soner
#define SONEROFFSET_X 		150 // [mm]
#define SONEROFFSET_ANGLE	(-45 * (M_PI / 180)) 			//rad
#define SONEROFFSET_ANGLE_L	((180-SONEROFFSET_ANGLE_R) * (M_PI / 180))	//[rad]
// bumper
#define BUMPER_PLACE_OFFSET 170 //[mm]
#define BUMPER_PLACE_ANGLE_R  (45 * (M_PI / 180))		//[rad]
#define BUMPER_PLACE_ANGLE_L  ((180-BUMPER_PLACE_ANGLE_R) * (M_PI / 180))

// 超音波センサで障害物の座標を記録する際の閾値　この距離より遠い座標は記録しない
#define RECORD_OBSTACLE_TH 500

/*****************************************************************************
 ** enum
 *****************************************************************************/
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


/*****************************************************************************
 ** Classes
 *****************************************************************************/
class Create
{
public:
	Create()
	{
		this->current_direction = PLUS_X;
		this->distance=0;
		this->velocity=0; 
		this->total_angle=0;
		this->push_bumper = NONE;
	}

// public method

	// 超音波センサを使って直進
	void goStraightWithSoner();

	// 走らせる（超音波センサ使用）
	void run();

	// 止まる
	void stopRun()
	{
		drive(0,0);
	}


	// Createの現在座標（world）を計算
	Coordinate calcCurrentCoordinate();
	Coordinate calcCurrentCoordinate(int distance, int angle);

	// 超音波センサが当たる部分の座標（world）の計算
	Coordinate calcSonerHitPointCoordinate(float soner_distance);

	// バンパーセンサが当たった部分の座標（world）の計算
	Coordinate calcBumperHitPointCoordinate();

	// 障害物に衝突したときの方向転換
	void changeDirection();

// checkState時に使用する関数
	// バンパー衝突時
	void doBumperHitMode(Coordinate &create, Coordinate &obstacle);
	// 通常時
	void doNormalMode(Coordinate &create, Coordinate &obstacle, float &soner_distance);


	// 超音波センサから距離値を得る[mm]	
	float getDistanceBySoner();



// set method
	void setDirection(enum Direction d)
	{
		this->current_direction = d;
	}

	// Createの角度を更新
	void addAngle(int angle)
	{
		this->total_angle += angle;
		this->total_angle = this->total_angle % 360;
	}
	// createの走行距離を更新
	void addDistance(int distance)
	{
		this->distance +=distance;
	}

	// 現在座標の値を更新
	void updateCurrentCoordinateXY(float X, float Y)
	{
		this->current_coord.setX(X);
		this->current_coord.setY(Y);
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

	Coordinate getCurrentCoordinate()
	{
		return this->current_coord;
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

	enum Direction current_direction;

	Coordinate current_coord;


};

#include "Create.cpp"
