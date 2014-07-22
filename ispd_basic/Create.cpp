
int Create::driveCreate(int velocity, int distance_max)
{
	int distance=0;
	drive(velocity, 0);
	while(1)
	{
		distance += getDistance();
		if(fabs(distance) >= distance_max)
		{
			break;
		}
	}
	return distance;
}

float Create::turnCreate(int velocity, int angle_max)
{
	int angle=0;
	drive(velocity, 1);
	while(1)
	{
		angle += getAngle();
		if(fabs(angle) >= angle_max)
		{
			break;
		}
	}

	if(angle >=0 )
	{
		return 0.0593*angle + 14.843 + angle;
	}
	else
	{
		return 0.04914*angle + -15.122 + angle;
	}
}


float Create::getAngleFromCreate( int velocity )
{
	int x;
	float angle;
	x = getAngle();
	if(this->StopTurn)	// Stop → Run
	{
		if(velocity == 200)
		{
			angle = 0.01329*x + 31.642 + x;
		}
		else if(velocity == -200)
		{
			angle = 0.01329*x - 31.642 + x;
		}
		else if(velocity == 100)
		{
			angle = 0.0593*x + 14.843 + x;
		}
		else if(velocity = -100)
		{
			angle = 0.04914*x + -15.122 + x;
		}
	}
	else	// Run → Run
	{
		if(velocity == 200)
		{
			angle = 0.01329*x + x;
		}
		else if(velocity == -200)
		{
			angle = 0.01329*x + x;
		}			
		else if(velocity == 100)
		{
			angle = 0.0593*x + x;
		}
		else if(velocity = -100)
		{
			angle = 0.04914*x + x;
		}
	}
	if(angle>360)
	{
		return angle-360;
	}
	else if(angle<-360)
	{
		return angle+360;
	}

	return angle;

}

void Create::addAngle(float angle)
{
	float current_angle;
	this->total_angle += angle;
	current_angle = this->total_angle;
	if(current_angle >= 360)
	{
		this->total_angle -= 360;
	}
	else if(current_angle <= -360 )
	{
		this->total_angle += 360;
	}
	current_angle = this->total_angle;
	if((current_angle < 45 && current_angle >= 0) || (current_angle < 0 && current_angle >= -45) || (current_angle < 360 && current_angle >= 315) || (current_angle < -315 && current_angle > -360))
	{
		this->direction = PLUS_X; 
	}
	else if( (current_angle < 135 && current_angle >= 45) || (current_angle < -225 && current_angle >= -315) )
	{
		this->direction = PLUS_Y;
	}
	else if( (current_angle < 225 && current_angle >= 135) || (current_angle < -135 && current_angle >= -225) )
	{
		this->direction = MINUS_X;
	}
	else if( (current_angle < 315 && current_angle >= 225) || (current_angle < -45 && current_angle >= -135) )
	{
		this->direction = MINUS_Y;
	}
}

//AnalogInputに接続した超音波センサから距離を取得するメソッド
float Create::getDistanceBySoner()
{
	int analog_input = 0;
	
	analog_input = readSensor(SENSOR_ANALOG_SIGNAL);	//アナログ値取得
	
	return ((6450.0/1023.0)*(float)analog_input);		//アナログ値[v]→距離[mm]		
}



void Create::goStraightWithSoner()
{
	//float total_soner=0.0;
//	float soner_distance = this->getDistanceBySoner();
	float soner_distance;
	//for(int i = 0;i<3;i++)
	//{
	//	total_soner += this->getDistanceBySoner();
//	}
	soner_distance = this->getDistanceBySoner();
std::cout << "soner_dist:" << this->getDistanceBySoner()  << std::endl;

	if((soner_distance >= WALL_DISTANCE_LOW) && (soner_distance <= WALL_DISTANCE_HIGH))
	{
		this->Straight_Run = true;
		drive(VELOCITY_H, 0);	//壁との距離が範囲内であれば直進
	}
	else if(soner_distance < WALL_DISTANCE_LOW)
	{
		this->Left_Run = true;
		drive(VELOCITY_H, RADIUS_LEFT);	//壁と近い場合、左向きに直進
	}
	else
	{
		this->Right_Run = true;
		drive(VELOCITY_H, -RADIUS_RIGHT);	//壁と遠い場合、右向きに直進
	}
}

void Create::checkDistToWall(float &ave_dist)
{
	float total = 0.0;
	this->soner_dist.push_back( this->getDistanceBySoner() );
	
	if(this->soner_dist.size()>5)
	{	
		for(int i=0;i<this->soner_dist.size();i++)
		{
			total += this->soner_dist[i];
		}
		ave_dist = total/this->soner_dist.size();
		this->soner_dist.clear();
	}

}

void Create::goStraight()
{
		this->Straight_Run = true;
		drive(VELOCITY_H,0);
}

void Create::goStraightWithoutSoner()
{
	float distance_to_wall;
	this->Straight_Run = true;
	this->checkDistToWall(distance_to_wall);

	if(distance_to_wall > 300)
	{
		int tmp_distance;
		tmp_distance = this->getDistanceFromCreate();
		this->stopRun();
		this->calcCurrentCoordinate(tmp_distance);
		this->StopRun = true;
		this->addAngle( this->turnCreate(VELOCITY_L, -15) );

	}
	if(distance_to_wall < 150)
	{
		int tmp_distance;
		tmp_distance = this->getDistanceFromCreate();
		this->stopRun();
		this->calcCurrentCoordinate(tmp_distance);
		this->StopRun = true;
		this->addAngle( this->turnCreate(VELOCITY_L, 15) );
		
	}
	drive(VELOCITY_H,0);
}


void Create::run()
{
	if(this->Stop)
	{
		this->StopRun = true;	// 発進フラグON
		this->Stop = false;
	}
//	this->goStraightWithSoner();
//	this->goStraightWithoutSoner();
	this->goStraight();
}

// 障害物に当たったら、方向転換する関数
void Create::changeDirection()
{
	float angle = this->total_angle;
	int turn_angle;

//std::cout << "angle : " << angle << std::endl;
	int tmp_distance;
	tmp_distance = driveCreate(-VELOCITY_H,50);	//5cm後退
	this->StopRun = true;
	this->calcCurrentCoordinate(tmp_distance);

	int current_angle = this->getTotalAngle(); // 現在の角度
	current_angle = current_angle % 90; // 現在の角度

	std::cout << "current angle" << current_angle << std::endl;

	// 現在の角度から回転する角度を計算
/*	if(current_angle < 45 || (current_angle >=315 && current_angle < 360))
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
	
		turn_angle = 90-current_angle;
	}
*/
		turn_angle = 15;

	this->push_bumper == NONE;
	std::cout << "turn angle" << turn_angle << std::endl;
	float tmp_angle;
	if(turn_angle >= 0)
	{
		tmp_angle = turnCreate(VELOCITY_L,turn_angle); // 半時計回り
		this->addAngle(tmp_angle); // turnCreateの返り値は、補正後の回転角度
	}
	else
	{
		turn_angle = 90;
		tmp_angle = turnCreate(VELOCITY_L,turn_angle); // 半時計回り
		this->addAngle(tmp_angle);
	}
	this->init();

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

	this->addAngle( this->getAngleFromCreate( VELOCITY_H ) );	// Create角度更新

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
Coordinate Create::calcCurrentCoordinate(int distance )
{
	float current_angle_r;//[rad]

	float tmp_x, tmp_y;   //計算用相対座標
	float distance_x, distance_y;   //計算用相対座標
	float pri_x, pri_y;   //1つ前に記録した座標

	current_angle_r = (float)this->getTotalAngle() * (M_PI / 180);
	float angle_r; //[rad]
	
	if(this->Straight_Run)
	{
		tmp_x = (float) distance * cos(current_angle_r);
		tmp_y = (float) distance * sin(current_angle_r);

		if(this->StopRun)
		{
			if(distance > 0)	// 前進の場合
			{
				distance_x = -0.01196*distance+66.97+distance;
				distance_y = -0.0242 * distance;	// 横ずれ
			}
			else				// 後退の場合
			{
				distance_x = -0.01196*distance-66.97+distance;
				distance_y = -0.0242 * distance;	// 横ずれ
			}
			this->StopRun = false;
		}
		else
		{	
				distance_x = -0.01196*distance+distance;
				distance_y = -0.0242 * distance;	// 横ずれ	
		}

		tmp_x = (float) distance_x * cos(current_angle_r) - distance_y * sin(current_angle_r);
		tmp_y = (float) distance_x * sin(current_angle_r) + distance_y * cos(current_angle_r);

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

		angle_r = ( (float)distance / (float)radius ); //[rad]
		tmp_x = 2.0 * radius * sin(angle_r/2.0) * cos(angle_r/2.0 + current_angle_r);
		tmp_y = 2.0 * radius * sin(angle_r/2.0) * sin(angle_r/2.0 + current_angle_r);
//std::cout << "coord" << tmp_x << ":" << tmp_y << std::endl;
//std::cout << "angler" << angle_r << std::endl;
//std::cout << "radius" << radius << std::endl;
//std::cout << "distance" << distance << std::endl;
//std::cout << "current_angle_r" << current_angle_r << std::endl;

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
	//int angle 	 = this->getAngleFromCreate( VELOCITY_H );	
	int angle 	 = getAngle();	
	this->stopRun();//値だけ取得してストップ

	this->soner_dist.clear(); // 壁にぶつかったので、今まで計測していたsoner_distをクリア

	this->addAngle(angle);	// Create角度更新
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
	create = this->calcCurrentCoordinate(distance);
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
	create = this->calcCurrentCoordinate(distance);
	obstacle = this->calcBumperHitPointCoordinate();

	this->changeDirection();	// 方向転換

}


// checState->通常時 
void Create::doNormalMode(Coordinate &create, Coordinate &obstacle, float &soner_distance)
{

		// distance取得
		int distance = this->getDistanceFromCreate( );
		this->addDistance(distance);
		// angle取得
		this->addAngle( getAngle());	// Create角度更新

		create = this->calcCurrentCoordinate( distance );

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
	drive (VELOCITY_H, 0);

	while (1)
	{
		usleep (20000);
		count += getDistanceFromCreate();
		angle += getAngleFromCreate( VELOCITY_H );
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
			create = this->calcCurrentCoordinate(count);
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
	float angle = acos(fabs(dist_x) / distance);
	float direction_angle = 0.0;
	int tmp_angle;
	int create_angle=this->total_angle * M_PI/180;
	
	if(dist_x<0 && dist_y>0)
	{
		angle +=M_PI/2;
	}
	else if(dist_x<0 && dist_y<0)
	{
		angle +=M_PI;
	}
	else if(dist_x>0 && dist_y<0)
	{
		angle += M_PI*1.5;
	}

	if(create_angle > angle)
	{
		if(create_angle-angle > M_PI)
		{
			direction_angle = 360-(create_angle-angle);
		}
		else
		{
			direction_angle = -(create_angle-angle);
		}
	}
	else
	{
		if(angle-create_angle > M_PI)
		{
			if(create_angle >= 0)
			{
				direction_angle = -(360-(angle-create_angle));
			}
			else
			{
				direction_angle = 360-(angle-create_angle);
			}
		}
		else
		{
			direction_angle = angle-create_angle;
		}

	}


	std::cout << "direction_anle" << direction_angle * ( 180.0/M_PI ) << std::endl;
	
	// direction_angleを誤差を踏まえた値に変換
	// 回転
	if(direction_angle>=0)
	{
		tmp_angle = turn(VELOCITY_H, 1, direction_angle, 0);
	}
	else
	{
		tmp_angle = turn(-VELOCITY_H, 1, direction_angle, 0);
	}
	// 角度の更新
	this->addAngle(tmp_angle);
	// 直進→障害物に当たった時,止まって5cm下がり、90度半時計回転後、障害物の座標値を得る
	this->driveDistanceSearchingObstacle(distance, create, obstacle, Bumper_Hit);

}

