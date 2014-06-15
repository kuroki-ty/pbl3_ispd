
// distance, angle coordinate, soner hit pointを記録


void Controller::checkState()
{
	Coordinate create;
	Coordinate obstacle;

	// 状態=RUN 	且つ バンパセンサ反応無し → 現在座標、超音波センサの観測座標を記録
	if(this->create.state == RUN && getBumpsAndWheelDrops() == 0)
	{
		float soner_distance;
		Coordinate pre_obstacle;

		this->create.doNormalMode(create, obstacle, soner_distance);
		this->tracks.push_back_MapPointList( create );	//createの現在座標を計算してプッシュバック
		
		// RECORD_OBSTACLE_TH 以上離れた障害物は記録しない
		if(soner_distance < RECORD_OBSTACLE_TH)
		{
			// 超音波センサの観測座標を計算してobstacle_listにプッシュバック
			this->tracks.push_back_ObstaclePointList( obstacle );
		}
		this->create.run();
	}
	
	// バンパセンサに衝突
	else if( getBumpsAndWheelDrops() != 0)
	{	
		// バンパーヒットモードに切り替え→現在座標とバンパーヒット位置を計算
		this->create.doBumperHitMode(create, obstacle);
		// 各座表の情報を渡す
		this->tracks.push_back_MapPointList( create );	
		this->tracks.push_back_ObstaclePointList( obstacle );
	}
}


