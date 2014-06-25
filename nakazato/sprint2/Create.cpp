
//AnalogInputに接続した超音波センサから距離を取得するメソッド
float Create::getDistanceBySoner()
{
	int analog_input = 0;
	
	analog_input = readSensor(SENSOR_ANALOG_SIGNAL);	//アナログ値取得
	
	return ((6450.0/1023.0)*(float)analog_input);		//アナログ値[v]→距離[mm]		
}



void Create::goStraightWithSoner()
{
	float total_soner=0.0;
//	float soner_distance = this->getDistanceBySoner();
	float soner_distance;
	for(int i = 0;i<3;i++)
	{
		total_soner += this->getDistanceBySoner();
	}
	soner_distance = total_soner/3;

	if((soner_distance >= WALL_DISTANCE_LOW) && (soner_distance <= WALL_DISTANCE_HIGH))
	{
		this->Straight_Run = true;
		drive(VELOCITY, 0);	//壁との距離が範囲内であれば直進
	}
	else if(soner_distance < WALL_DISTANCE_LOW)
	{
		this->Left_Run = true;
		drive(VELOCITY, RADIUS_LEFT);	//壁と近い場合、左向きに直進
	}
	else
	{
		this->Right_Run = true;
		drive(VELOCITY, -RADIUS_RIGHT);	//壁と遠い場合、右向きに直進
	}
	std::cout << "Create Start!" << std::endl;
}


void Create::run()
{
	this->goStraightWithSoner();
	//drive(VELOCITY, 0);
	//this->Straight_Run = true;
}

// 障害物に当たったら、方向転換する関数
void Create::changeDirection()
{
	int angle = this->total_angle;
	int turn_angle;

//std::cout << "angle : " << angle << std::endl;
	int tmp_distance;
	tmp_distance = driveDistance(-VELOCITY2,0,-50,0);	//5cm後退
	this->calcCurrentCoordinate(tmp_distance, this->getAngleFromCreate());

	int current_angle = this->getTotalAngle() % 90;
/*
	if(this->push_bumper == RIGHT )
	{	
		turn_angle = (90-BUMPER_PLACE_ANGLE_R);
	}
	else if(this->push_bumper == LEFT )
	{	
		turn_angle = (180-BUMPER_PLACE_ANGLE_L);
	}
	else if(this->push_bumper == CENTER )
	{	
		turn_angle = 90;
	}
*/

	if(current_angle < 45 || (current_angle >=315 && current_angle < 360))
	{
		turn_angle = 90-current_angle;
	}
	else if(current_angle >= 45 && current_angle < 135 )
	{
		turn_angle = 180-current_angle;
	}
	else if(current_angle >= 135 && current_angle < 225)
	{
		turn_angle = 270-current_angle;
	}
	else if(current_angle >=225 && current_angle < 315)
	{
		turn_angle = 90-current_angle;
	}


	this->push_bumper == NONE;
	
	turn_angle -= current_angle; 
	//turn_angle /=1.244;

	int tmp_angle;
	if(turn_angle >= 0)
	{
		tmp_angle = turn(VELOCITY,1,turn_angle,0); // 半時計回り
		this->addAngle(tmp_angle);
	}
	else
	{
		turn_angle = 90;
		tmp_angle = turn(VELOCITY,1,turn_angle,0); // 半時計回り
		this->addAngle(tmp_angle);
	}
	//this->total_angle = 0;
/*
	if(this->direction == PLUS_X)
	{
		this->total_angle = 90;
	}
	else if(this->direction == MINUS_X)
	{
		this->total_angle = 270;
	}
	else if(this->direction == PLUS_Y)
	{
		this->total_angle = 180;
	}
	else if(this->direction == MINUS_Y)
	{
		this->total_angle = 0;
	}
*/


	// turnangleの誤差考慮
	std::cout << "turn_angle : " << tmp_angle << std::endl;
}


// Createの現在座標の計算関数
Coordinate Create::calcCurrentCoordinate()
{
	int distance = this->getDistanceFromCreate();
	this->addDistance(distance);
	//int last_angle = this->getTotalAngle();
	float current_angle_r;// [rad]

	float tmp_x, tmp_y; //計算用相対座標
	
	float pri_x, pri_y;
	
	//float crt_angle_r = (float) last_angle * (M_PI / 180); //createの角度[rad]

	this->addAngle( this->getAngleFromCreate() );	// Create角度更新

	float angle_r; //[rad]
	current_angle_r = (float)this->getTotalAngle() * (M_PI / 180);

	if(this->Straight_Run)
	{
		tmp_x = (float) distance * cos(current_angle_r);
		tmp_y = (float) distance * sin(current_angle_r);
		this->Straight_Run = false;
	}
	else
	{
		float radius;	// 回転半径
		if  (this->Right_Run) 
		{
			radius = RADIUS_RIGHT;
			this->Right_Run = false;
		}
		else//(this->Left_Run)
		{
			radius = RADIUS_LEFT;
			this->Left_Run = false;
		}

		angle_r = (float)( distance / radius ); //[rad]
		tmp_x = 2 * radius * sin(angle_r/2) * cos(angle_r/2 + current_angle_r);
		tmp_y = 2 * radius * sin(angle_r/2) * sin(angle_r/2 + current_angle_r);
//std::cout << "tmp:(" << tmp_x << ", " << tmp_y << ")" << (float)current_angle_r *(180/M_PI) << std::endl;

	}

	pri_x= this->current_coord.x;	//map_point_list内の最新の座標を得る（前回記録した位置）
	pri_y= this->current_coord.y;

	this->updateCurrentCoordinateXY( pri_x + tmp_x, pri_y + tmp_y );	// Createの現在座標を更新

	return this->getCurrentCoordinate();
}

// 距離と角度を引数にした、Createの現在座標の計算関数
Coordinate Create::calcCurrentCoordinate(int distance, int angle)
{
	//int last_angle = this->getTotalAngle();
	float current_angle_r;//[rad]

	float tmp_x, tmp_y; //計算用相対座標
	float pri_x, pri_y; //1つ前に記録した座標
	
	//float crt_angle_r = (float) last_angle * (M_PI / 180); //createの角度[rad]

	this->addAngle( angle );	// Create角度更新
	current_angle_r = (float)this->getTotalAngle() * (M_PI / 180);
	float angle_r; //[rad]

	if(this->Straight_Run)
	{
		tmp_x = (float) distance * cos(current_angle_r);
		tmp_y = (float) distance * sin(current_angle_r);
		this->Straight_Run = false;
	}
	else
	{
		int radius;	// 回転半径
		if  (this->Right_Run) 
		{
			radius = RADIUS_RIGHT;
			this->Right_Run = false;
		}
		else//(this->Left_Run)
		{
			radius = RADIUS_LEFT;
			this->Left_Run = false;
		}

		angle_r = (float)( distance / radius ); //[rad]
		tmp_x = 2 * radius * sin(angle_r/2) * cos(angle_r/2 + current_angle_r);
		tmp_y = 2 * radius * sin(angle_r/2) * sin(angle_r/2 + current_angle_r);
	}


	pri_x= this->current_coord.x;	//map_point_list内の最新の座標を得る（前回記録した位置）
	pri_y= this->current_coord.y;

	this->updateCurrentCoordinateXY( pri_x + tmp_x , pri_y + tmp_y );	// Createの現在座標を更新

	return this->getCurrentCoordinate();
}


/************************************************************
* 超音波センサが当たったところのCreateからの相対座標を計算
* 入力1：Createの現在のworld座標
* 入力2:Createの現在のworld回転角度
* 返り値：障害物座標（world）
*************************************************************/
Coordinate Create::calcSonerHitPointCoordinate(float soner_distance)
{
	
	Coordinate obstacle_coord;
	Coordinate create_coord;
	create_coord = this->getCurrentCoordinate();
	float create_x = create_coord.x;
	float create_y = create_coord.y;

	int create_angle_r = (float)this->getTotalAngle() * (M_PI / 180);

	float obstacle_x = create_x + SONEROFFSET_X * cos(create_angle_r) + soner_distance * cos(create_angle_r + SONEROFFSET_ANGLE);
	float obstacle_y = create_y + SONEROFFSET_X * sin(create_angle_r) + soner_distance * sin(create_angle_r + SONEROFFSET_ANGLE); 

	obstacle_coord.x = obstacle_x;
	obstacle_coord.y = obstacle_y;

	return obstacle_coord;

}

Coordinate Create::calcBumperHitPointCoordinate()
{
	Coordinate obstacle_coord;
	Coordinate create_coord;
	create_coord = this->getCurrentCoordinate();
	float create_angle_r = (float)this->getTotalAngle() * (M_PI / 180);
	float create_x = create_coord.x;
	float create_y = create_coord.y;
	float obstacle_x, obstacle_y;

	// 右バンパが押された場合
	if(this->push_bumper == RIGHT)
	{
		obstacle_x = create_x + BUMPER_PLACE_OFFSET * cos(BUMPER_PLACE_ANGLE_R + create_angle_r);
		obstacle_y = create_y + BUMPER_PLACE_OFFSET * sin(BUMPER_PLACE_ANGLE_R + create_angle_r);
	}
	// 左バンパが押された場合
	else if(this->push_bumper == LEFT)
	{
		obstacle_x = create_x + BUMPER_PLACE_OFFSET * cos(BUMPER_PLACE_ANGLE_L + create_angle_r);
		obstacle_y = create_y + BUMPER_PLACE_OFFSET * sin(BUMPER_PLACE_ANGLE_L + create_angle_r);
		this->push_bumper = LEFT;
	}
	// 正面のバンパが押された場合
	else
	{
		obstacle_x = create_x + BUMPER_PLACE_OFFSET * sin(create_angle_r);
		obstacle_y = create_y + BUMPER_PLACE_OFFSET * cos(create_angle_r);
	}
	obstacle_coord.x = obstacle_x;
	obstacle_coord.y = obstacle_y;

	return obstacle_coord;

}


// checState->バンパーに衝突
void Create::doBumperHitMode(int bumper_hit, Coordinate &create, Coordinate &obstacle)
{
	int distance = this->getDistanceFromCreate();
	int angle 	 = this->getAngleFromCreate();

	this->stopRun();//値だけ取得してストップ
	this->addDistance(distance);// Distanceを更新、Angleは後のcalculateCreateCoordinate（）内で更新

	if(bumper_hit == 1)
	{
		this->push_bumper = RIGHT;
	}
	else if(bumper_hit == 2)
	{
		this->push_bumper = LEFT;
	}
	else
	{
		this->push_bumper = CENTER;
	}
	
	// 各座標の計算
	create = this->calcCurrentCoordinate(distance, angle);
	obstacle = this->calcBumperHitPointCoordinate();

	this->changeDirection();	// 方向転換

}

void Create::doBumperHitModeAtObstacleSerch(int distance, int angle, int bumper_hit, Coordinate &create, Coordinate &obstacle)
{
	this->stopRun();
	this->addDistance(distance);// Distanceを更新、Angleは後のcalculateCreateCoordinate（）内で更新

	if(bumper_hit == 1)
	{
		this->push_bumper = RIGHT;
	}
	else if(bumper_hit == 2)
	{
		this->push_bumper = LEFT;
	}
	else
	{
		this->push_bumper = CENTER;
	}
	
	// 各座標の計算
	create = this->calcCurrentCoordinate(distance, angle);
	obstacle = this->calcBumperHitPointCoordinate();

	this->changeDirection();	// 方向転換

}


// checState->通常時 
void Create::doNormalMode(Coordinate &create, Coordinate &obstacle, float &soner_distance)
{
		create = this->calcCurrentCoordinate();

		soner_distance = this->getDistanceBySoner();	
		// RECORD_OBSTACLE_TH 以上離れた障害物は記録しない
		if(soner_distance < RECORD_OBSTACLE_TH)
		{
			// 超音波センサの観測座標を計算してobstacle_listにプッシュバック
			obstacle = this->calcSonerHitPointCoordinate(soner_distance);
		}
//		this->run();

}


void Create::driveDistanceSearchingObstacle(int distance, Coordinate &create, Coordinate &obstacle, bool &Bumper_Hit)
{

	int count = 0;
	int angle = 0;
	int bumper_hit;
std::cout << "distance:" << distance << std::endl;
	drive (VELOCITY, 0);

	while (1)
	{
		usleep (20000);
		count += getDistanceFromCreate();
		angle += getAngleFromCreate();
std::cout << "count:" << count << std::endl;
		bumper_hit = getBumpsAndWheelDrops();
		if( bumper_hit!=0)// バンパーが反応したら
		{
			Bumper_Hit = true;
			this->doBumperHitModeAtObstacleSerch(count, angle, bumper_hit, create, obstacle);
			break;
		}
		
		if ((distance >= 0 && count >= distance) || (distance < 0 && count <= distance))
		{
			create = this->calcCurrentCoordinate(count, angle);
			break;
		}
	}


}




/*
*	runAlongPointList( ムーブポイントのリスト )
*
*
*
*/
void Create::runNextPoint(Coordinate move_point, bool &Bumper_Hit, Coordinate &create, Coordinate &obstacle)
{

	// 進む方向を計算（現在座標、現在の角度、次の座標、）→出力 回転角
	// 進む距離を計算（現在座標、次の座標）→出力 距離
	float dist_x = move_point.x - this->current_coord.x;
	float dist_y = move_point.y - this->current_coord.y;
	int distance = sqrt( dist_x*dist_x + dist_y*dist_y );
	float angle = acos(dist_x / distance);
	float direction_angle = this->total_angle * ( M_PI/180.0 ) + angle;
	int tmp_angle;

	std::cout << "direction_anle" << direction_angle * ( M_PI/180.0 ) << std::endl;
	
	// direction_angleを誤差を踏まえた値に変換
	// 回転
	if(direction_angle>=0)
	{
		tmp_angle = turn(VELOCITY, 1, direction_angle, 0);
	}
	else
	{
		tmp_angle = turn(-VELOCITY, 1, direction_angle, 0);
	}
	// 角度の更新
	this->addAngle(tmp_angle);
	// 直進→障害物に当たった時,止まって5cm下がり、90度半時計回転後、障害物の座標値を得る
	this->driveDistanceSearchingObstacle(distance, create, obstacle, Bumper_Hit);

}

