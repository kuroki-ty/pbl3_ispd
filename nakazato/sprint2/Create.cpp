


#define CREATE_SIZE 340 // 縦 [mm]
// 横330[mm]

#define SONEROFFSET_X 		150 // [mm]
#define SONEROFFSET_ANGLE	45 //degree


//AnalogInputに接続した超音波センサから距離を取得するメソッド
double getDistanceBySoner()
{
	int analog_input = 0;
	
	analog_input = readSensor(SENSOR_ANALOG_SIGNAL);	//アナログ値取得
	
	return ((645.0/1023.0)*(double)analog_input);		//アナログ値[v]→距離[cm]		
}
/************************************************************
* 超音波センサが当たったところの座標を計算
* 入力1：Createの現在のworld座標
* 入力2:Createの現在のworld回転角度
* 返り値：障害物座標（wotld）
*************************************************************/
Coordinate Create::sonerHitPoint()
{
	Coordinate obstacle_coord;
	int create_x = this->coordinate.getX();
	int create_y = this->coordinate.getY();
	int create_angle = this->angle;

	int obstacle_x = create_x + SONEROFFSET_X*cos(create_angle) + getDistanceBySoner()*cos(create_angle + SONEROFFSET_ANGLE);
	int obstacle_y = create_y + SONEROFFSET_X*sin(create_angle) + getDistanceBySoner()*sin(create_angle + SONEROFFSET_ANGLE); 

	obstacle_coord.setX(obstacle_x);
	obstacle_coord.setY(obstacle_y);

	return obstacle_coord;

}


void Create::goStraightWithSoner()
{
	if((getDistanceBySoner() >= WALL_DISTANCE_LOW) && (getDistanceBySoner() <= WALL_DISTANCE_HIGH))
	{
		this->Straight_Run = true;
		drive(VELOCITY, 0);	//壁との距離が範囲内であれば直進
	}
	else if(getDistanceBySoner() < WALL_DISTANCE_LOW)
	{
		this->Left_Run = true;
		drive(VELOCITY, RADIUS_LEFT);	//壁と近い場合、左向きに直進
	}
	else
	{
		this->Right_Run = true;
		drive(VELOCITY, -RADIUS_RIGHT);	//壁と遠い場合、右向きに直進
	}
}


void runWithSoner( double distance ) // 次に行くための距離（誤差修正済）が引数
{

}

void Create::run()
{
	this->goStraightWithSoner();
}

// 障害物に当たったので、方向転換する関数
void Create::changeDirection()
{
	int angle = getAngle();
	int turn_angle;

	this->angle += angle;
	angle = this->angle;
	std::cout << "angle : " << angle << std::endl;
	driveDistance(-VELOCITY,0,-100,0);	//10cm後退

	angle = angle % 90;

	if(this->present_direction == PLUS_X )
	{	
		turn_angle = 90-angle;
		this->present_direction = PLUS_Y;
	}
	else if(this->present_direction == MINUS_X )
	{	
		turn_angle = 90-angle;
		this->present_direction = MINUS_Y;
	}
	else if(this->present_direction == PLUS_Y )
	{	
		turn_angle = 90-angle;
		this->present_direction = MINUS_X;
	}
	else if(this->present_direction == MINUS_Y )
	{	
		turn_angle = 90-angle;
		this->present_direction = PLUS_X;
	}

// turnangleの誤差考慮
std::cout << "turn_angle : " << turn_angle << std::endl;
	this->angle += getAngle();
	if(turn_angle >= 0)
	{
		turn(VELOCITY,1,turn_angle,0); // 半時計回り
		this->angle += turn_angle;
	}
	else
	{
		turn(VELOCITY,1,90,0); // 半時計回り
		this->angle += 90;
	}
}
