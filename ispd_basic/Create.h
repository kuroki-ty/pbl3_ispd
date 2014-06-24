#include <createoi.h>
#include "Coordinate.h"


/*****************************************************************************
 ** Define
 *****************************************************************************/
#define VELOCITY 200			//直進速度[mm/s]
#define VELOCITY2 100			//直進速度[mm/s]
#define RADIUS_LEFT  230.0		//左回り回転半径[mm]
#define RADIUS_RIGHT 320.0		//右回り回転半径[mm]
#define WALL_DISTANCE_LOW  140.0	//壁とCreateの距離_低[cm]
#define WALL_DISTANCE_HIGH 180.0	//壁とCreateの距離_高[cm]

#define CREATE_SIZE 330 // 縦 [mm]
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
		this->direction = PLUS_X;
		this->total_distance=0;
		this->velocity=0; 
		this->total_angle=0;
		this->push_bumper = NONE;
		this->current_coord.x = 165.0;
		this->current_coord.y = 165.0;
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
	void doBumperHitMode(int bumper_hit, Coordinate &create, Coordinate &obstacle);
	void doBumperHitModeAtObstacleSerch(int distance, int angle, int bumper_hit, Coordinate &create, Coordinate &obstacle);

	// 通常時
	void doNormalMode(Coordinate &create, Coordinate &obstacle, float &soner_distance);


	// 超音波センサから距離値を得る[mm]	
	float getDistanceBySoner();

	void driveDistanceSearchingObstacle(int distance, Coordinate &create, Coordinate &obstacle, bool &Bumper_Hit);
	void runNextPoint(Coordinate move_point, bool &Bumper_Hit, Coordinate &create, Coordinate &obstacle);



// set method
	// Createの角度を更新
	void addAngle(int angle)
	{
		this->total_angle += angle;
		this->total_angle = this->total_angle % 360;
		if((this->total_angle < 45 && this->total_angle >= 0) || (this->total_angle < 0 && this->total_angle >= -45) || (this->total_angle < 360 && this->total_angle >= 315) || (this->total_angle < -315 && this->total_angle > -360))
		{
			this->direction = PLUS_X; 
		}
		else if( (this->total_angle < 135 && this->total_angle >= 45) || (this->total_angle < -225 && this->total_angle >= -315) )
		{
			this->direction = PLUS_Y;
		}
		else if( (this->total_angle < 225 && this->total_angle >= 135) || (this->total_angle < -135 && this->total_angle >= -225) )
		{
			this->direction = MINUS_X;
		}
		else if( (this->total_angle < 315 && this->total_angle >= 225) || (this->total_angle < -45 && this->total_angle >= -135) )
		{
			this->direction = MINUS_Y;
		}
	}
	// createの走行距離を更新
	void addDistance(int distance)
	{
		this->total_distance +=distance;
	}

	// 現在座標の値を更新
	void updateCurrentCoordinateXY(float X, float Y)
	{
		this->current_coord.x = X;
		this->current_coord.y = Y;
	}


// get method
	int getTotalAngle()	
	{
		return this->total_angle;
	}
	int getTotalDistance()	
	{
		return this->total_distance;
	}


	int getAngleFromCreate()
	{
		int x;
		//x = (int)(getAngle()*1.244) % 360;
		x = getAngle() % 360;
		return x;
	}

	int getDistanceFromCreate()
	{
		float x= 0.0;
		float y= 0.0;
		x = getDistance();
		//y = 7*(10e-08)*x*x*x - 2*(10e-04)*x*x + 0.1305*x;
		
		return (int)(x-y);
	}	

	Coordinate getCurrentCoordinate()
	{
		return this->current_coord;
	}



//public 変数
	bool Straight_Run;
	bool Right_Run;
	bool Left_Run;
	enum Bumper push_bumper;
	enum IRobotDirection direction;

private:
	int velocity;
	int total_angle;
	int total_distance;

	Coordinate current_coord;


};

#include "Create.cpp"
