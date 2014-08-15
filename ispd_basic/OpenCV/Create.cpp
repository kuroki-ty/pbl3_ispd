
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
	this->init();
	this->init();
	drive(velocity, 1);
	while(1)
	{
		angle += getAngle();
		if(fabs(angle) >= fabs(angle_max))
		{
			drive(0,0);
			this->stopRun();
			break;
		}
	}
	waitTime(0.2);
	if(angle >0 )
	{
			std::cout <<"turn------------------------:" << P_TURN_A100 * angle * angle + P_TURN_B100 * angle + P_TURN_C100+ angle << std::endl;
			return P_TURN_A100 * angle * angle + P_TURN_B100 * angle + P_TURN_C100 + angle;
	}
	else if(angle == 0)
	{
			std::cout <<"turn------------------------:" << P_TURN_A100 * angle << std::endl;
			return P_TURN_A100 * angle;
	}
	else
	{
		//if(fabs(angle) < 10)
		//{
			std::cout <<"turn------------------------:" << -P_TURN_A100 * angle * angle + P_TURN_B100 * angle - P_TURN_C100 + angle << std::endl;
			return -P_TURN_A100 * angle * angle + P_TURN_B100 * angle - P_TURN_C100 + angle;
	//	}
		//else
	//	{
		//	std::cout <<"turn------------------------:" << N_TURN_A100 * angle * angle + N_TURN_B100 * angle + N_TURN_C100 + angle << std::endl;
		//	return N_TURN_A100 * angle * angle + N_TURN_B100 * angle + N_TURN_C100 + angle;			
	//	}
	}
}

/***********************
*
* 	超音波センサで座標をとりながら回転
*	返り値：取得した座標
*
*
************************/
float Create::turnCreateGettingObstacleCoord(int velocity, int angle_max, std::vector<Coordinate> &obstacle_coord )
{

	int angle=0;
	this->init();
	float soner_distance;;
	drive(velocity, 1);
	while(1)
	{

		soner_distance = this->getDistanceBySoner();
		if(soner_distance < RECORD_OBSTACLE_TH)
		{
			obstacle_coord.push_back(this->calcSonerHitPointCoordinate(soner_distance));
		}

		angle += getAngle();
		std::cout << angle << std::endl;
		if(fabs(angle) >= fabs(angle_max))
		{
			this->stopRun();
			break;
		}
	}
//	waitTime(0.0);
	if(angle >0 )
	{
			return P_TURN_A100 * angle * angle + P_TURN_B100 * angle + P_TURN_C100;
	}
	else if(angle == 0)
	{
			return P_TURN_A100 * angle;
	}
	else
	{
			return N_TURN_A100 * angle * angle + N_TURN_B100 * angle + N_TURN_C100;
	}
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



void Create::goStraightCurveWithSoner()
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
	float soner_distance = this->getDistanceBySoner();
	ave_dist = AVE_DIST;
	std::cout << "soner--------------------------:" << soner_distance << std::endl;
	if(soner_distance < WALL_SEARCH_TH)
	{
		this->soner_dist.push_back( soner_distance );
	}
	
	if(this->soner_dist.size()>3)
	{	
		for(int i=0;i<this->soner_dist.size();i++)
		{
			total += this->soner_dist[i];
		}
		ave_dist = total/this->soner_dist.size();
	std::cout << "ave_dist-------------------------:" << ave_dist << std::endl;
		this->soner_dist.clear();
	}
}

void Create::goStraight()
{
		this->Straight_Run = true;
		drive(VELOCITY_H,0);
}

void Create::goStraightTurnWithSoner()
{
	float distance_to_wall;
	this->Straight_Run = true;
	this->checkDistToWall(distance_to_wall);

	if(distance_to_wall > WALL_DIST_H)
	{
		int tmp_distance;
		tmp_distance = this->getDistanceFromCreate();
		this->stopRun();
		this->addDistance(tmp_distance);
		this->calcCurrentCoordinate(tmp_distance);
		this->addAngle( this->turnCreate(-VELOCITY_L, RETURN_ANGLE) );
	}
	if(distance_to_wall < WALL_DIST_L)
	{
		int tmp_distance;
		tmp_distance = this->getDistanceFromCreate();
		this->stopRun();
		this->addDistance(tmp_distance);
		this->calcCurrentCoordinate(tmp_distance);
		this->addAngle( this->turnCreate(VELOCITY_L, RETURN_ANGLE) );	
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
	//this->goStraightCurveWithSoner();
	this->goStraightTurnWithSoner();
	//this->goStraight();
}

// 障害物に当たったら、方向転換する関数
void Create::changeDirection()
{
	float angle = this->total_angle;
	int turn_angle;

//std::cout << "angle : " << angle << std::endl;
	int tmp_distance;
	tmp_distance = driveCreate(-VELOCITY_H,100);	//5cm後退
	this->addDistance(tmp_distance);
	this->StopRun = true;
	this->calcCurrentCoordinate(tmp_distance);
	this->stopRun();

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

	if(this->push_bumper == LEFT)
	{
		turn_angle = 108;
	}
	else if(this->push_bumper == RIGHT )
	{
		turn_angle = 38;
	}
	else if(this->push_bumper == CENTER )
	{
		turn_angle = 79;
	}
	else
	{
		turn_angle = 90;
	}
	turn_angle = 79;

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
	this->stopRun();

}


// Createの現在座標の計算関数
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
			if(distance >= 0)	// 前進の場合
			{
				distance_x = P_DRIVEX_A200*distance+P_DRIVEX_B200+distance;
				distance_y = P_DRIVEY_A200 * distance;	// 横ずれ
			}
			else				// 後退の場合
			{
				distance_x = N_DRIVEX_A100*distance+N_DRIVEX_B100;
				distance_y = N_DRIVEY_A100;// * distance;	// 横ずれ
			}
			this->StopRun = false;

		}
		else
		{	
				distance_x = P_DRIVEX_A200 * distance + distance;
				distance_y = P_DRIVEY_A200 * distance;	// 横ずれ	
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
	this->stopRun();//値だけ取得してストップ

	this->soner_dist.clear(); // 壁にぶつかったので、今まで計測していたsoner_distをクリア

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

std::vector<Coordinate> Create::searchObstacle(std::vector<Coordinate> &create)
{
	float soner_distance;
	std::vector<Coordinate> obstacle_coord;
	std::vector<Coordinate> turn_obstacle_coord;

	int bumper_hit;	
	float soner_ave[5]={0.0,0.0,0.0,0.0,0.0};

	int count=0;
	int dist;

	// 発進
	drive(VELOCITY_H, 0);
	int k=0;
	while(count < 1000)
	{
		dist = getDistance();
		count += dist;
		// 超音波センサで取得
std::cout << "angle===============" << this->total_angle <<  std::endl;
		soner_ave[k%5] = this->getDistanceBySoner();
if(k>5)
{
		for(int i=0;i<5;i++)
		{
			soner_distance +=soner_ave[i];
			soner_ave[i] = 0.0;
		}
		k=0;		

		soner_distance /=5.0; 
		// 一定値内であれば、障害物として登録
		if(soner_distance < RECORD_OBSTACLE_TH)
		{
			obstacle_coord.push_back(this->calcSonerHitPointCoordinate(soner_distance));
		}

//std::cout << "soner_obstacle_dist================" << soner_distance << std::endl;
		// 壁がある値以上離れたら止まる
		if(soner_distance > OBSTACLE_SEARCH_H_TH || soner_distance < OBSTACLE_SEARCH_L_TH)
		{
			this->stopRun();
			// 20度回転
			turn_obstacle_coord.clear();
			if(soner_distance > OBSTACLE_SEARCH_H_TH)
			{
				this->addAngle (this->turnCreateGettingObstacleCoord(-VELOCITY_L, 15, turn_obstacle_coord));
			}
			else
			{
				this->addAngle (this->turnCreateGettingObstacleCoord(VELOCITY_L, 15, turn_obstacle_coord));
			}
			// 回転中に取得した障害物座標を登録
			for(int i=0;i<turn_obstacle_coord.size();i++)
			{
				obstacle_coord.push_back(turn_obstacle_coord[i]);
			}
			int tmp_distance;
			tmp_distance = driveCreate(VELOCITY_H,10);	//5cm前進
			this->addDistance(tmp_distance);
			this->StopRun = true;
			create.push_back(this->calcCurrentCoordinate(tmp_distance));
			count += tmp_distance;

			drive(VELOCITY_H, 0);

		}

}
		bumper_hit = getBumpsAndWheelDrops();
		if( bumper_hit!=0)// バンパーが反応したら
		{
			this->stopRun();
			create.push_back(this->calcCurrentCoordinate( dist ));
			Coordinate tmp_create;
			Coordinate tmp_obstacle;

			this->doBumperHitMode(bumper_hit, tmp_create, tmp_obstacle);
			create.push_back(tmp_create);
			obstacle_coord.push_back(tmp_obstacle);
			drive(VELOCITY_L, 0);
		}
		else
		{
			create.push_back(this->calcCurrentCoordinate( dist ));
		}
	// 発進
	k++;
	}

	return obstacle_coord;

}


void Create::doBumperHitModeAtObstacleSerch(int distance, int angle, int bumper_hit, Coordinate &create, Coordinate &obstacle)
{
	this->stopRun();
//	this->addDistance(distance);// Distanceを更新、Angleは後のcalculateCreateCoordinate（）内で更新

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
	
	// 障害物の周りを回る　超音波センサをつかって、障害物を記録
	//this->searchObstacle();


}


// checState->通常時 
void Create::doNormalMode(Coordinate &create, Coordinate &obstacle, float &soner_distance)
{

		// distance取得
		int distance = this->getDistanceFromCreate( );
		this->addDistance(distance);
		// angle取得
		//this->addAngle( getAngle());	// Create角度更新

		create = this->calcCurrentCoordinate( distance );

		soner_distance = this->getDistanceBySoner();	
		// RECORD_OBSTACLE_TH 以上離れた障害物は記録しない
		if(soner_distance < RECORD_OBSTACLE_TH)
		{
			// 超音波センサの観測座標を計算してobstacle_listにプッシュバック
			obstacle = this->calcSonerHitPointCoordinate(soner_distance);
		}

}


void Create::driveDistanceSearchingObstacle(int distance, std::vector<Coordinate> &create, std::vector<Coordinate> &obstacle, bool &Bumper_Hit)
{

	int count = 0;
	int bumper_hit;
	int dist;
	float soner_distance;

std::cout << "distance:" << distance << std::endl;
	drive (VELOCITY_H, 0);

	while (1)
	{
		usleep (20000);
		dist = getDistanceFromCreate();
		count += dist;

		bumper_hit = getBumpsAndWheelDrops();
		if( bumper_hit!=0)// バンパーが反応したら
		{
			this->stopRun();
			create.push_back(this->calcCurrentCoordinate( dist ));
			Coordinate tmp_create;
			Coordinate tmp_obstacle;

			this->addDistance(count);
			Bumper_Hit = true;
			this->doBumperHitMode(bumper_hit, tmp_create, tmp_obstacle);
			create.push_back(tmp_create);
			obstacle.push_back(tmp_obstacle);

			break;
		}
		// 超音波センサを使って障害物の位置をサーチ
		soner_distance = this->getDistanceBySoner();	
		// RECORD_OBSTACLE_TH 以上離れた障害物は記録しない
		if(soner_distance < RECORD_OBSTACLE_TH)
		{
			// 超音波センサの観測座標を計算してobstacle_listにプッシュバック
			obstacle.push_back(this->calcSonerHitPointCoordinate(soner_distance));
		}
		
		if ((distance >= 0 && count >= distance) || (distance < 0 && count <= distance))
		{
			this->stopRun();
			dist = getDistanceFromCreate();
			count += dist;
			create.push_back(this->calcCurrentCoordinate( dist ));
			this->addDistance(count);
			break;
		}
		create.push_back(this->calcCurrentCoordinate( dist ));
	}


}




/*
*	runAlongPointList( ムーブポイントのリスト )
*
*
*
*/
void Create::runNextPoint(Coordinate move_point, bool &Bumper_Hit, std::vector<Coordinate> &create, std::vector<Coordinate> &obstacle)
{

	// 進む方向を計算（現在座標、現在の角度、次の座標、）→出力 回転角
	// 進む距離を計算（現在座標、次の座標）→出力 距離
	float dist_x = move_point.x - this->current_coord.x;
	float dist_y = move_point.y - this->current_coord.y;
	int distance = sqrt( dist_x*dist_x + dist_y*dist_y );
	float angle = acos(fabs(dist_x) / distance);
	float direction_angle = 0.0;
	int tmp_angle;
	int create_angle=this->total_angle;

	std::cout << "angle前---------------------------------(" << angle * ( 180.0/M_PI ) << std::endl;		
	if(dist_x<0 && dist_y<0)
	{
		angle =M_PI+angle;
	}
	else if(dist_x<0 && dist_y>0)
	{
		angle =M_PI-angle;
	}
	else if(dist_x>0 && dist_y<0)
	{
		angle =2*M_PI-angle;
	}
	std::cout << "angle後---------------------------------(" << angle * ( 180.0/M_PI ) << std::endl;	

	angle = angle*( 180.0/M_PI );


	if(create_angle < 0)
	{
		create_angle +=360;
		this->addAngle(360);
	}


	if(create_angle > angle)
	{
		if(create_angle-angle > 180)
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
		if(angle-create_angle > 180)
		{
			direction_angle = -(360-(angle-create_angle));
		}
		else
		{
			direction_angle = angle-create_angle;
		}
	}

/*
else
{
		if(angle-create_angle > 180 && angle-create_angle < 360)
		{
			direction_angle = -(360-(angle-create_angle));
		}
		else if(angle-create_angle >= 360)
		{
			direction_angle = 720-(angle-create_angle);
			if(direction_angle > 180)
			{
				direction_angle = 360 - direction_angle;
			}
		}
		else
		{
			direction_angle = angle-create_angle;
		}
}
*/

	if(fabs(direction_angle) <1.0)
	{
		direction_angle = 0.0;
	}

	std::cout << "direction_angle前---------------------------------(" << direction_angle << std::endl;	
	// direction_angleを誤差を踏まえた値に変換
	if(direction_angle>0)
	{
		direction_angle = (direction_angle - P_TURN_B100)/(P_TURN_A100);  
	}
	else if(direction_angle==0)
	{
		direction_angle = 0.0;  
	}
	else
	{
		direction_angle = (direction_angle - N_TURN_B100)/(N_TURN_A100);
	}
	std::cout << "direction_angle後---------------------------------(" << direction_angle << std::endl;	
	// 回転
	if(direction_angle>=0)
	{
		tmp_angle = turnCreate(VELOCITY_L, direction_angle);
	}
	else
	{
		tmp_angle = turnCreate(-VELOCITY_L, direction_angle);
	}
	// 角度の更新
	this->addAngle(tmp_angle);
	// 直進→障害物に当たった時,止まって5cm下がり、90度半時計回転後、障害物の座標値を得る
	this->driveDistanceSearchingObstacle(distance, create, obstacle, Bumper_Hit);

}

bool Create::isDockFound()
{
	bool dock_f = false;
	int sensor_value;

	sensor_value = readSensor(SENSOR_INFRARED);
	switch(sensor_value){
	case 248:
		printf("Red Buoy!\n");
		dock_f = true;
		break;
	case 244:
		printf("Green Buoy!\n");
		dock_f = true;
		break;
	case 242:
		printf("Force Field!\n");
		dock_f = false;
		break;
	case 252:
		printf("Red Buoy and Green Buoy!\n");
		dock_f = true;
		break;
	case 250:
		printf("Red Buoy and Force Field!\n");
		dock_f = true;
		break;
	case 246:
		printf("Green Buoy and Force Field!\n");
		dock_f = true;
		break;
	case 254:
		printf("Red Buoy, Green Buoy and Force Field!\n");
		dock_f = true;
		break;
	default:
		dock_f = false;
		break;
	}
	
	return dock_f;
}
