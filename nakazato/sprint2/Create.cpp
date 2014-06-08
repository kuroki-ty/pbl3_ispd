
//AnalogInputに接続した超音波センサから距離を取得するメソッド
float Create::getDistanceBySoner()
{
	int analog_input = 0;
	
	analog_input = readSensor(SENSOR_ANALOG_SIGNAL);	//アナログ値取得
	
	return ((6450.0/1023.0)*(float)analog_input);		//アナログ値[v]→距離[mm]		
}



void Create::goStraightWithSoner()
{
	if((this->getDistanceBySoner() >= WALL_DISTANCE_LOW) && (this->getDistanceBySoner() <= WALL_DISTANCE_HIGH))
	{
		this->Straight_Run = true;
		drive(VELOCITY, 0);	//壁との距離が範囲内であれば直進
	}
	else if(this->getDistanceBySoner() < WALL_DISTANCE_LOW)
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


void Create::run()
{
	this->goStraightWithSoner();
	this->state = RUN;
}

// 障害物に当たったので、方向転換する関数
void Create::changeDirection()
{
	int angle = this->total_angle;
	int turn_angle;

//std::cout << "angle : " << angle << std::endl;

	driveDistance(-VELOCITY,0,-50,0);	//5cm後退

	

	if(this->push_bumper == RIGHT )
	{	
		turn_angle = 90-BUMPER_PLACE_ANGLE_R;
		this->push_bumper == NONE;
	}
	else if(this->push_bumper == LEFT )
	{	
		turn_angle = 180-BUMPER_PLACE_ANGLE_L;
		this->push_bumper == NONE;
	}
	else if(this->push_bumper == CENTER )
	{	
		turn_angle = 90;
		this->push_bumper == NONE;
	}


// turnangleの誤差考慮
//std::cout << "turn_angle : " << turn_angle << std::endl;
	this->total_angle += this->getAngleFromCreate();
	if(turn_angle >= 0)
	{
		turn(VELOCITY,1,turn_angle,0); // 半時計回り
		this->total_angle += turn_angle;
	}
	else
	{
		turn(VELOCITY,1,90,0); // 半時計回り
		this->total_angle += 90;
	}
}
