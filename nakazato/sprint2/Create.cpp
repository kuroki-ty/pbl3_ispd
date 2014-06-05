
#include <createoi.h> 



//AnalogInputに接続した超音波センサから距離を取得するメソッド
double getDistanceBySoner()
{
	int analog_input = 0;
	
	analog_input = readSensor(SENSOR_ANALOG_SIGNAL);	//アナログ値取得
	
	return ((645.0/1023.0)*(double)analog_input);		//アナログ値[v]→距離[cm]		
}





void goStraightWithSoner()
{
	if((getDistanceBySoner() >= WALL_DISTANCE_LOW) && (getDistanceBySoner() <= WALL_DISTANCE_HIGH))
	{
		drive(VELOCITY, 0);	//壁との距離が範囲内であれば直進
	}
	else if(getDistanceBySoner() < WALL_DISTANCE_LOW)
	{
		drive(VELOCITY, RADIUS_LEFT);	//壁と近い場合、左向きに直進
	}
	else
	{
		drive(VELOCITY, -RADIUS_RIGHT);	//壁と遠い場合、右向きに直進
	}
}


void runWithSoner( double distance ) // 次に行くための距離（誤差修正済）が引数
{

}

void Create::Run()
{
	goStraightWithSoner();
}

// 障害物に当たったので、方向転換する関数
void Create::changeDirection()
{
	int angle = getAngle();
	int turn_angle;
	std::cout << "angle : " << angle << std::endl;

	driveDistance(-VELOCITY,0,-100,0);	//10cm後退

	if(this->direction == PLUS_X )
	{	
		turn_angle = 90-getAngle();
	}
	else if(this->direction == MINUS_X )
	{	
		turn_angle = 270-getAngle();
	}
	else if(this->direction == PLUS_Y )
	{	
		turn_angle = 180-getAngle();
	}
	else if(this->direction == MINUS_X )
	{	
		turn_angle = 360-getAngle();
	}
// turnangleの誤差考慮
	turn(VELOCITY,0,turn_angle,0); // 半時計回り


}
