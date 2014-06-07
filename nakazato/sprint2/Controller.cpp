

/*
void state(enum State state)
{

	if(event == STOP)
	{
		this->create.runWithSoner();
	}
	else if(event == RUN)
	{

	}
	else if(event == TURN)
	{

	}
	else if(event == BUMPER)
	{

	}

}
*/
// distance, angle coordinate, soner hit pointを記録
Coordinate Controller::calculateCreateCoordinate()
{
	int distance = getDistance();
	int last_angle = this->create.angle;

	float tmp_x, tmp_y; //計算用相対座標
	float tmp_x1, tmp_x2, tmp_y1, tmp_y2;	// 計算用
	float pri_x, pri_y; //1つ前に記録した座標
	
	float crt_angle_r = (float) this->create.getCreateAngle() * (PI / 180); //createの角度[rad]

	this->create.addAngle( getAngle() );	// Create角度更新

	float angle_r; //[rad]

	if(this->create.Straight_Run)
	{
		tmp_x = (float) distance * cos(this->create.angle);
		tmp_y = (float) distance * sin(this->create.angle);
		this->create.Straight_Run = false;
	}
	else
	{
		int radius;	// 回転半径
		if  (Right_Run) 
		{
			radius = RADIUS_RIGHT;
			this->create.Right_Run = false;
		}
		else(Left_Run)
		{
			radius = LEFT_RUN;
			this->create.Left_Run
		}

		angle_r = ( distance / radius );
		tmp_x1 = radius * sin( angle_r + crt_angle_r);
		tmp_x2 = radius * sin( crt_angle_r );
		tmp_x = tmp_x1 - tmp_x2;

		tmp_y1 = 2 * radius * sin( ( angle_r + crt_angle_r ) / 2 ) * sin( ( angle_r + crt_angle_r ) / 2 );
		tmp_y2 = 2 * radius * sin( crt_angle_r / 2 ) * sin( crt_angle_r / 2 );
		tmp_y = tmp_y1 - tmp_y2;
	}

	pri_x= this->tracks.map_point_list[this->tracks.map_point_list.end()].getX();
	pri_y= this->tracks.map_point_list[this->tracks.map_point_list.end()].getY();

	this->create.present_coord.setX( pri_x + tmp_x );	// Createの現在座標を記録
	this->create.present_coord.setY( pri_y + tmp_y );	//

	return this->create.present_coord;
}

void Controller::checkState()
{
	// 状態=RUN 	且つ バンパセンサ反応無し
	if(this->create.state == RUN && getBumpsAndWheelDrops() == 0)
	{
		this->recordCoordinate( this->calculateCreateCoordinate() );	//createのworld座標を計算してtracksに記録
		this->create.run();
	}

	else if( getBumpsAndWheelDrops() != 0)
	{	
		this->calculateCreateCoordinate();
		this->create.changeDirection();	// バンパーに当たったので方向転換モードに移行
	}
}


