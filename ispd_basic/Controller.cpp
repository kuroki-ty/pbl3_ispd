/*
*
*
*
*
*/
void Controller::checkState()
{
	Coordinate create_coord;
	Coordinate obstacle_coord;

	bool Bumper_Hit = false;
std::cout << "distance:" << this->create.getTotalDistance() << std::endl;
std::cout << "angle:" << this->create.getTotalAngle() << std::endl;
std::cout << "(" << this->create.getCurrentCoordinate().x << ", " << this->create.getCurrentCoordinate().y << ")" << std::endl;
std::cout << "direction:"<< this->create.direction << std::endl;
	this->block.showMesh();

// 1.壁探索
	if(this->search_flag == WALL)// wall探索フラグをfalseにする処理を入れる
	{
		int bumper_hit = getBumpsAndWheelDrops();

//1-1.バンパーに衝突したかを判定
// 1-1-1.バンパセンサ反応無し → 現在座標、超音波センサの観測座標を記録
		if(bumper_hit == 0)
		{
			float soner_distance;
			Coordinate pre_obstacle_coord;
			// 1-1-1-1各座表値の計算
			this->create.doNormalMode(create_coord, obstacle_coord, soner_distance);
			// 1-1-1-2座標値の記録 → Map		
			this->map.push_back_CreatePointList( create_coord );	//createの現在座標を記録（プッシュバック）					
			if(soner_distance < RECORD_OBSTACLE_TH) // RECORD_OBSTACLE_TH 以上離れた障害物は記録しない
			{
				this->map.push_back_ObstaclePointList( obstacle_coord ); // 障害物の座標を記録（プッシュバック）
			}
			this->create.run();
		}	
// 1-1-2.バンパセンサに衝突
		else if( bumper_hit != 0)
		{	
			// 1-1-2-1.現在座標とバンパーヒット座標を計算 ＆ 方向転換 → Createクラスへ
			this->create.doBumperHitMode(bumper_hit, create_coord, obstacle_coord);

			// 1-1-2-2.座標値の記録 → Mapクラスへ
			this->map.push_back_CreatePointList( create_coord );	
			this->map.push_back_ObstaclePointList( obstacle_coord );
		}
//1-2.メッシュの更新
		this->block.setMeshMark( create_coord, Bumper_Hit );

//1-3.壁探索終了の判定
		Coordinate start_coord;
		int total_distance = this->create.getTotalDistance();
	    if(this->block.isStartMesh( start_coord, create_coord,  total_distance))
		{
			this->search_flag = OBSTACLE;
		}
	}

//2.障害物探索
	else if(this->search_flag == OBSTACLE) // 障害物探索
	{
std::cout << "！！！！！！！！！！！！！！！！！！！！！！！！！！！！１" << std::endl;
		int goal;
		std::vector<Coordinate> move_point_list;  // Createが辿る座標を格納したリスト
// 2-1.createの現在座標を取得
		create_coord = this->create.getCurrentCoordinate();
// 2-2.createの現在座標から向かうメッシュを計算し、辿る座標リストを得る
		goal = this->block.getNextMeshNum();
std::cout << "！！！！！！！！！！！！！！！！！！！！！！！！！！！！2" << std::endl;
		move_point_list = this->block.getMovePointList(create_coord, this->create.direction, goal);// → Createの現在座標を元に、ゴールまでに辿る座標のリストを得る
// 2-3.座標リスト通りに進んでいく
std::cout << "！！！！！！！！！！！！！！！！！！！！！！！！！！！！3" << std::endl;
		for(int i=0;i<move_point_list.size();i++)
		{
std::cout << "！！！！！！！！！！！！！！！！！！！！！！！！！！！！4" << move_point_list.size() << std::endl;
			// 2-3-1.次の座標値を渡して、移動と、各座表値の計算を行う
			this->create.runNextPoint(move_point_list[i], Bumper_Hit, create_coord, obstacle_coord); //
std::cout << "！！！！！！！！！！！！！！！！！！！！！！！！！！！！5" << std::endl;
			// 2-3-2 移動後の座標値を渡して、メッシュを更新
			this->block.setMeshMark(create_coord, Bumper_Hit);// 
std::cout << "！！！！！！！！！！！！！！！！！！！！！！！！！！！！6" << std::endl;
			// 2-3-3各座表の情報を渡して、map,obstacleリストを更新
			this->map.push_back_CreatePointList( create_coord );
std::cout << "！！！！！！！！！！！！！！！！！！！！！！！！！！！！7" << std::endl;
			if(Bumper_Hit)
			{	
				this->map.push_back_ObstaclePointList( obstacle_coord );
			}
		}
std::cout << "！！！！！！！！！！！！！！！！！！！！！！！！！！！！8" << std::endl;
// 2-4.全部のメッシュをチェックし終えたかを判定
		if( this->block.checkAllSearchEnd() )
		{
			this->search_flag = DOCK;

		}
	}

// 3.ドッキングステーションへゴール
	else if(this->search_flag == DOCK)
	{
		this->create.stopRun();
		this->finished = true;
		std::cout << "finished!" << std::endl;
	}



}


