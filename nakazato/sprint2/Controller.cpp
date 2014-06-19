
// distance, angle coordinate, soner hit pointを記録


void Controller::checkState()
{
	Coordinate create;
	Coordinate obstacle;

	if(wall_serch)// wall探索フラグをfalseにする処理を入れる
	{
		// 状態=RUN 	且つ バンパセンサ反応無し → 現在座標、超音波センサの観測座標を記録
		if(this->create.state == RUN && getBumpsAndWheelDrops() == 0)
		{
			float soner_distance;
			Coordinate pre_obstacle;

			this->create.doNormalMode(create, obstacle, soner_distance);
			this->map.push_back_CreatePointList( create );	//createの現在座標を計算してプッシュバック
		
			// RECORD_OBSTACLE_TH 以上離れた障害物は記録しない
			if(soner_distance < RECORD_OBSTACLE_TH)
			{
				// 超音波センサの観測座標を計算してobstacle_listにプッシュバック
				this->map.push_back_ObstaclePointList( obstacle );
			}
			this->create.run();
		}
	
		// バンパセンサに衝突
		else if( getBumpsAndWheelDrops() != 0)
		{	
			// バンパーヒットモードに切り替え→現在座標とバンパーヒット位置を計算
			this->create.doBumperHitMode(create, obstacle);
			// 各座表の情報を渡す
			this->map.push_back_CreatePointList( create );	
			this->map.push_back_ObstaclePointList( obstacle );
			// blockに登録

		}
	}
	if else(Obstacle_Search) // 障害物探索
	{
		Coordinate move_point_list;  // Createが辿る座標を格納したリスト
		bool Bumper_Hit = false;

//		this->block.getNextMovePoint(next_move_point, this->create.getCurrentCoord());// → Createの現在座標を元に、ゴールまでに辿る座標のリストを得る

		for(int i=0;i<move_point_list.size();i++)
		{
//			this->create.runNextPoint(move_point_list[i], Bumper_Hit); // 　
	//		this->block.updatateBlockList(this->create.getCurrentCoord(), Bumper_Hit);// 

			// 各座表の情報を渡して、map,obstacleリストを更新
			this->map.push_back_CreatePointList( create );
			if(Bumper_Hit)
			{	
				this->map.push_back_ObstaclePointList( obstacle );
			}
		}
	}
	if else(Docking) // ドッキングステーションへ
	{


	}



}


