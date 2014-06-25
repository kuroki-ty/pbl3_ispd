
// distance, angle coordinate, soner hit pointを記録


void Controller::checkState()
{
	Coordinate create_coord;
	Coordinate obstacle_coord;

//std::cout << "distance:" << this->create.getTotalDistance() << std::endl;
std::cout << "angle:" << this->create.getTotalAngle() << std::endl;
std::cout << "(" << this->create.getCurrentCoordinate().x << ", " << this->create.getCurrentCoordinate().y << ")" << std::endl;
std::cout << "direction:"<< this->create.direction << std::endl;

this->angle.push_back(this->create.getTotalAngle());

	if(this->search_flag == WALL)// wall探索フラグをfalseにする処理を入れる
	{
		int bumper_hit = getBumpsAndWheelDrops();

		// 状態=RUN 	且つ バンパセンサ反応無し → 現在座標、超音波センサの観測座標を記録
		if(bumper_hit == 0)
		{
			float soner_distance;
			Coordinate pre_obstacle_coord;
			this->create.doNormalMode(create_coord, obstacle_coord, soner_distance);
			this->map.push_back_CreatePointList( create_coord );	//createの現在座標を計算してプッシュバック
std::cout << "soner:"<< soner_distance << std::endl;		
			// RECORD_OBSTACLE_TH 以上離れた障害物は記録しない
			if(soner_distance < RECORD_OBSTACLE_TH)
			{
				// 超音波センサの観測座標を計算してobstacle_listにプッシュバック
				this->map.push_back_ObstaclePointList( obstacle_coord );
			}
			this->create.run();
		}	
		// バンパセンサに衝突
		else if( bumper_hit != 0)
		{	
			// バンパーヒットモードに切り替え→現在座標とバンパーヒット位置を計算の後、方向転換
			this->create.doBumperHitMode(bumper_hit, create_coord, obstacle_coord);
			// 各座表の情報を渡す
			this->map.push_back_CreatePointList( create_coord );	
			this->map.push_back_ObstaclePointList( obstacle_coord );
			// blockに登録

		}
//		if(this->block.isStartMesh(create_coord) )
		if(this->map.getCreateListSize() > 120 )
		{
			this->finished = true;
		}
std::cout << std::endl;
	}
	else if(this->search_flag == OBSTACLE) // 障害物探索
	{
		std::vector<Coordinate> move_point_list;  // Createが辿る座標を格納したリスト
		bool Bumper_Hit = false;
		create_coord = this->create.getCurrentCoordinate();
//		this->block.getMovePointList(move_point_list, create_coord);// → Createの現在座標を元に、ゴールまでに辿る座標のリストを得る

		for(int i=0;i<move_point_list.size();i++)
		{
			this->create.runNextPoint(move_point_list[i], Bumper_Hit, create_coord, obstacle_coord); //
			
	//		this->block.setMeshMark(create_coord, Bumper_Hit);// 
			// 各座表の情報を渡して、map,obstacleリストを更新
			this->map.push_back_CreatePointList( create_coord );
			if(Bumper_Hit)
			{	
				this->map.push_back_ObstaclePointList( obstacle_coord );
			}
		}
	}


/*	if else(Docking) // ドッキングステーションへ
	{


	}
*/


}


