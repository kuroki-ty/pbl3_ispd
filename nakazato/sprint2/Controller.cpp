

// distance, angle coordinate, soner hit pointを記録

// Createの現在座標の計算関数
Coordinate Controller::calculateCreateCoordinate()
{
	int distance = this->create.getDistanceFromCreate();
	this->create.addDistance(distance);
	int last_angle = this->create.getTotalAngle();
	float current_angle_r;// [rad]

	float tmp_x, tmp_y; //計算用相対座標
	float tmp_x1, tmp_x2, tmp_y1, tmp_y2;	// 計算用
	float pri_x, pri_y; //1つ前に記録した座標
	
	float crt_angle_r = (float) last_angle * (M_PI / 180); //createの角度[rad]

	this->create.addAngle( this->create.getAngleFromCreate() );	// Create角度更新

	float angle_r; //[rad]

	if(this->create.Straight_Run)
	{
		current_angle_r = (float)this->create.getTotalAngle() * (M_PI / 180);
		tmp_x = (float) distance * cos(current_angle_r);
		tmp_y = (float) distance * sin(current_angle_r);
		this->create.Straight_Run = false;
	}
	else
	{
		int radius;	// 回転半径
		if  (this->create.Right_Run) 
		{
			radius = RADIUS_RIGHT;
			this->create.Right_Run = false;
		}
		else//(this->create.Left_Run)
		{
			radius = RADIUS_LEFT;
			this->create.Left_Run = false;
		}

		angle_r = ( distance / radius ); //[rad]
		tmp_x1 = radius * sin( angle_r + crt_angle_r);
		tmp_x2 = radius * sin( crt_angle_r );
		tmp_x = tmp_x1 - tmp_x2;

		tmp_y1 = 2 * radius * sin( ( angle_r + crt_angle_r ) / 2 ) * sin( ( angle_r + crt_angle_r ) / 2 );
		tmp_y2 = 2 * radius * sin( crt_angle_r / 2 ) * sin( crt_angle_r / 2 );
		tmp_y = tmp_y1 - tmp_y2;
	}

	pri_x= this->tracks.getCurrentMapPoint_X();	//map_point_list内の最新の座標を得る（前回記録した位置）
	pri_y= this->tracks.getCurrentMapPoint_Y();

	this->create.updatePresentCoordinateXY( pri_x + tmp_x, pri_y + tmp_y );	// Createの現在座標を更新

	return this->create.getPresentCoordinate();
}

// 距離と角度を引数にした、Createの現在座標の計算関数
Coordinate Controller::calculateCreateCoordinate(int distance, int angle)
{
	int last_angle = this->create.getTotalAngle();
	float current_angle_r;//[rad]

	float tmp_x, tmp_y; //計算用相対座標
	float tmp_x1, tmp_x2, tmp_y1, tmp_y2;	// 計算用
	float pri_x, pri_y; //1つ前に記録した座標
	
	float crt_angle_r = (float) last_angle * (M_PI / 180); //createの角度[rad]

	this->create.addAngle( angle );	// Create角度更新

	float angle_r; //[rad]

	if(this->create.Straight_Run)
	{
		current_angle_r = this->create.getTotalAngle();
		tmp_x = (float) distance * cos(current_angle_r);
		tmp_y = (float) distance * sin(current_angle_r);
		this->create.Straight_Run = false;
	}
	else
	{
		int radius;	// 回転半径
		if  (this->create.Right_Run) 
		{
			radius = RADIUS_RIGHT;
			this->create.Right_Run = false;
		}
		else//(this->create.Left_Run)
		{
			radius = RADIUS_LEFT;
			this->create.Left_Run = false;
		}

		angle_r = ( distance / radius );
		tmp_x1 = radius * sin( angle_r + crt_angle_r);
		tmp_x2 = radius * sin( crt_angle_r );
		tmp_x = tmp_x1 - tmp_x2;

		tmp_y1 = 2 * radius * sin( ( angle_r + crt_angle_r ) / 2 ) * sin( ( angle_r + crt_angle_r ) / 2 );
		tmp_y2 = 2 * radius * sin( crt_angle_r / 2 ) * sin( crt_angle_r / 2 );
		tmp_y = tmp_y1 - tmp_y2;
	}

	pri_x= this->tracks.getCurrentMapPoint_X();	//map_point_list内の最新の座標を得る（前回記録した位置）
	pri_y= this->tracks.getCurrentMapPoint_Y();

	this->create.updatePresentCoordinateXY( pri_x + tmp_x, pri_y + tmp_y );	// Createの現在座標を更新

	return this->create.getPresentCoordinate();
}


/************************************************************
* 超音波センサが当たったところのCreateからの相対座標を計算
* 入力1：Createの現在のworld座標
* 入力2:Createの現在のworld回転角度
* 返り値：障害物座標（world）
*************************************************************/
Coordinate Controller::calculateSonerHitPointCoordinate(float soner_distance)
{
	Coordinate obstacle_coord;
	Coordinate create_coord;
	create_coord = this->create.getPresentCoordinate();
	float create_x = create_coord.getX();
	float create_y = create_coord.getY();
	int create_angle_r = (float)this->create.getTotalAngle() * (M_PI / 180);

	float obstacle_x = create_x + SONEROFFSET_X * cos(create_angle_r) + soner_distance * cos(create_angle_r + SONEROFFSET_ANGLE);
	float obstacle_y = create_y + SONEROFFSET_X * sin(create_angle_r) + soner_distance * sin(create_angle_r + SONEROFFSET_ANGLE); 

	obstacle_coord.setX(obstacle_x);
	obstacle_coord.setY(obstacle_y);

	return obstacle_coord;

}

Coordinate Controller::calculateBumperHitPointCoordinate()
{
	Coordinate obstacle_coord;
	Coordinate create_coord;
	create_coord = this->create.getPresentCoordinate();
	float create_angle_r = (float)this->create.getTotalAngle() * (M_PI / 180);
	float create_x = create_coord.getX();
	float create_y = create_coord.getY();
	float obstacle_x, obstacle_y;

	// 右バンパが押された場合
	if(getBumpsAndWheelDrops() == 1)
	{
		obstacle_x = create_x + BUMPER_PLACE_OFFSET * cos(BUMPER_PLACE_ANGLE_R + create_angle_r);
		obstacle_y = create_y + BUMPER_PLACE_OFFSET * sin(BUMPER_PLACE_ANGLE_R + create_angle_r);
		this->create.push_bumper = RIGHT;
	}
	// 左バンパが押された場合
	else if(getBumpsAndWheelDrops() == 2)
	{
		obstacle_x = create_x + BUMPER_PLACE_OFFSET * cos(BUMPER_PLACE_ANGLE_L + create_angle_r);
		obstacle_y = create_y + BUMPER_PLACE_OFFSET * sin(BUMPER_PLACE_ANGLE_L + create_angle_r);
		this->create.push_bumper = LEFT;
	}
	// 正面のバンパが押された場合
	else
	{
		obstacle_x = create_x + BUMPER_PLACE_OFFSET * sin(create_angle_r);
		obstacle_y = create_y + BUMPER_PLACE_OFFSET * cos(create_angle_r);
		this->create.push_bumper = CENTER;	
	}
	obstacle_coord.setX(obstacle_x);
	obstacle_coord.setY(obstacle_y);

	return obstacle_coord;

}



void Controller::checkState()
{
	// 状態=RUN 	且つ バンパセンサ反応無し
	if(this->create.state == RUN && getBumpsAndWheelDrops() == 0)
	{
		this->tracks.push_back_MapPointList( this->calculateCreateCoordinate() );	//createのworld座標を計算してtracksに逐次記録
		float soner_distance = this->create.getDistanceBySoner();
//std::cout << "soner_dist:" << soner_distance << std::endl;
		// RECORD_OBSTACLE_TH 以上離れた障害物は記録しない
		if(soner_distance < RECORD_OBSTACLE_TH)
		{
			this->tracks.push_back_ObstaclePointList( this->calculateSonerHitPointCoordinate( soner_distance ) );
		}
		this->create.run();
	}
	
	// バンパセンサに衝突
	else if( getBumpsAndWheelDrops() != 0)
	{	
		int distance = this->create.getDistanceFromCreate();
		int angle 	 = this->create.getAngleFromCreate();
		this->create.stopRun();//値だけ取得してストップ
		this->create.addDistance(distance);// Distanceを更新、Angleは後のcalculateCreateCoordinate（）内で更新
		this->tracks.push_back_MapPointList( this->calculateCreateCoordinate(distance, angle) );
		this->tracks.push_back_ObstaclePointList( this->calculateBumperHitPointCoordinate() );
		this->create.changeDirection();	// 方向転換
	}
}


