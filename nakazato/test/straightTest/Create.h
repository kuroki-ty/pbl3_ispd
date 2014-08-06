#include <createoi.h>
#include "Coordinate.h"


/*****************************************************************************
 ** Define
 *****************************************************************************/
#define VELOCITY_H 200			//速度[mm/s]
#define VELOCITY_L 100			//速度[mm/s]
#define RADIUS_LEFT  230.0		//左回り回転半径[mm]
#define RADIUS_RIGHT 320.0		//右回り回転半径[mm]
#define WALL_DISTANCE_LOW  180.0	//壁とCreateの距離_低[cm]
#define WALL_DISTANCE_HIGH 230.0	//壁とCreateの距離_高[cm]

#define WALL_DIST_L 200	//壁とCreateの距離_低[cm]
#define WALL_DIST_H 400	//壁とCreateの距離_高[cm]


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

// 壁探索時の飛び値対策
#define WALL_SEARCH_TH 600

// 壁探索の直進回転走行の復帰回転角度
#define RETURN_ANGLE 4

// 回転誤差
#define P_TURN_A100 1.1002386
#define P_TURN_B100 2.247255

#define N_TURN_A100 1.1002386
#define N_TURN_B100 (-2.247255)


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
		this->current_coord.x = 0.0;
		this->current_coord.y = 0.0;
		this->Straight_Run = true;
		this->Right_Run = false;
		this->Left_Run  = false;
		this->Stop = true;
		this->StopRun = false;
		this->StopTurn = false;

	}

	void init()
	{
		std::cout << "distance & angle init" << std::endl;
		std::cout << getDistance() << std::endl;
		std::cout << getAngle() << std::endl;
	}

// public method

	// 超音波センサを使ってストレートとカーブで直進
	void goStraightCurveWithSoner();

	// 超音波センサを使って壁との距離を計測
	void checkDistToWall(float &ave_dist);

	// 超音波センサを使って直進と回転の繰り返しで進む
	void goStraightTurnWithSoner();

	// 	普通に走る
	void goStraight();

	// 走らせる（超音波センサ使用）
	void run();

	// 止まる
	void stopRun()
	{
		this->Stop = true;
		drive(0,0);
		this->init();
	}

	int driveCreate(int velocity, int distance_max);
	float turnCreate(int velocity, int angle_max);


	// Createの現在座標（world）を計算
	Coordinate calcCurrentCoordinate();
	Coordinate calcCurrentCoordinate(int distance );

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

	void driveDistanceSearchingObstacle(int distance, std::vector<Coordinate> &create, std::vector<Coordinate>&obstacle, bool &Bumper_Hit);
	void runNextPoint(Coordinate move_point, bool &Bumper_Hit, std::vector<Coordinate> &create, std::vector<Coordinate> &obstacle);

	// 障害物を探索するメソッド
	void searchObstacle();



// set method
	// Createの角度を更新
	void addAngle(float angle);

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
	float getTotalAngle()	
	{
		return this->total_angle;
	}
	int getTotalDistance()	
	{
		return this->total_distance;
	}


	float getAngleFromCreate( int velocity );

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
	bool Stop;
	bool StopRun;
	bool StopTurn;
	enum Bumper push_bumper;
	enum IRobotDirection direction;


	std::vector<float> soner_dist;

private:
	int velocity;
	float total_angle;
	int total_distance;

	Coordinate current_coord;


};

#include "Create.cpp"
