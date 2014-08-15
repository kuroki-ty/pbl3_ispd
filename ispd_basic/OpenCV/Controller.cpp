void Controller::showWall4()
{
			/*壁のメッシュを埋める処理 yamasaki記述*/
			/*setMeshMarksメソッド(壁探索後に壁のメッシュを埋めるメソッド)で使用する変数*/
			std::vector<Coordinate> p;		//壁直線の交点を格納する配列
    		std::vector<bool> xflag;		//壁直線がy=ax+bかy=cかを判定する変数
    		std::vector< std::vector<float> > ransac;	//壁直線の係数を格納する2次元配列 ransac[i][0]:a, ransac[i][1]:b ransac[i][2]:c

			this->map.calcLine();	//Mapクラスのobstacleリストから直線式を計算する
std::cout << "--------------------------2----------------------------" << std::endl;
			p = this->map.getIntersectionLine();	//直線の交点をgetする
std::cout << "--------------------------3----------------------------" << std::endl;
    		ransac = this->map.getCoefficientLine(xflag);	//直線式の係数をgetする
std::cout << "--------------------------4----------------------------" << std::endl;
			this->map.showMap2();
std::cout << "--------------------------5----------------------------" << std::endl;    
    		this->block.setMeshMarks(p, ransac, xflag);		//壁のメッシュをまとめて埋める
std::cout << "--------------------------6----------------------------" << std::endl;
			this->block.fillMesh();							//壁のメッシュの外側を全て埋める
std::cout << "--------------------------7----------------------------" << std::endl;
			this->block.showMesh();
			/***************************************************/
}

void Controller::showWall2()
{
			/*壁のメッシュを埋める処理 yamasaki記述*/
			/*setMeshMarksメソッド(壁探索後に壁のメッシュを埋めるメソッド)で使用する変数*/
			std::vector<Coordinate> p;		//壁直線の交点を格納する配列
    		std::vector<bool> xflag;		//壁直線がy=ax+bかy=cかを判定する変数
    		std::vector< std::vector<float> > ransac;	//壁直線の係数を格納する2次元配列 ransac[i][0]:a, ransac[i][1]:b ransac[i][2]:c

			this->map.calcLine();	//Mapクラスのobstacleリストから直線式を計算する
std::cout << "--------------------------2----------------------------" << std::endl;
			p = this->map.getIntersectionLine();	//直線の交点をgetする
std::cout << "--------------------------3----------------------------" << std::endl;
    		ransac = this->map.getCoefficientLine(xflag);	//直線式の係数をgetする
std::cout << "--------------------------4----------------------------" << std::endl;
			this->map.showMap2();
std::cout << "--------------------------5----------------------------" << std::endl;    
    		this->block.setMeshMarks(p, ransac, xflag);		//壁のメッシュをまとめて埋める
std::cout << "--------------------------6----------------------------" << std::endl;
			this->block.fillMesh();							//壁のメッシュの外側を全て埋める
std::cout << "--------------------------7----------------------------" << std::endl;
			this->block.showMesh();
			/***************************************************/
}


/*
*
*
*
*
*/
void Controller::checkState()
{
	Coordinate create_coord;

	bool Bumper_Hit = false;
std::cout << "total_distance:" << this->create.getTotalDistance() << std::endl;
std::cout << "angle:" << this->create.getTotalAngle() << std::endl;
std::cout << "(" << this->create.getCurrentCoordinate().x << ", " << this->create.getCurrentCoordinate().y << ")" << std::endl;
std::cout << "direction:"<< this->create.direction << std::endl;
	this->block.showMesh();

std::cout << "current mesh:" << this->block.getCurrentMeshNum(this->create.getCurrentCoordinate()) << std::endl;


// 1.壁探索
	if(this->search_flag == WALL)
	{
		int bumper_hit = getBumpsAndWheelDrops();
		Coordinate wall_coord;
//1-1.バンパーに衝突したかを判定
// 1-1-1.バンパセンサ反応無し → 現在座標、超音波センサの観測座標を記録
		if(bumper_hit == 0)
		{
			float soner_distance;
			// 1-1-1-1各座表値の計算
			this->create.doNormalMode(create_coord, wall_coord, soner_distance);
			// 1-1-1-2座標値の記録 → Map		
			this->map.push_back_CreatePointList( create_coord );	//createの現在座標を記録（プッシュバック）					
			if(soner_distance < RECORD_OBSTACLE_TH) // RECORD_OBSTACLE_TH 以上離れた障害物は記録しない
			{
				this->map.push_back_WallPointList( wall_coord ); // 障害物の座標を記録（プッシュバック）
			}
			this->create.run();
		}	
// 1-1-2.バンパセンサに衝突
		else if( bumper_hit != 0)
		{	
			// 1-1-2-1.現在座標とバンパーヒット座標を計算 ＆ 方向転換 → Createクラスへ
			this->create.doBumperHitMode(bumper_hit, create_coord, wall_coord);

			// 1-1-2-2.座標値の記録 → Mapクラスへ
			this->map.push_back_CreatePointList( create_coord );	
			this->map.push_back_WallPointList( wall_coord );
		}
//1-2.メッシュの更新
		this->block.setMeshMark( create_coord, Bumper_Hit );

//1-3.壁探索終了の判定
		Coordinate start_coord;
		int total_distance = this->create.getTotalDistance();
#if 0
	    if(this->block.isStartMesh( start_coord, create_coord,  total_distance))
		{
std::cout << "--------------------------1----------------------------" << std::endl;
			this->create.stopRun();// 壁探索が終わったら、即Createを止める
			this->search_flag = OBSTACLE;

			// 壁を描画する
			this->showWall2();
		}
#endif
		if(this->create.getTotalDistance() > 6000)
		{
			this->create.stopRun();// 壁探索が終わったら、即Createを止める
			this->showWall2();
			this->search_flag = DOCK;
			this->create.stopRun();// 壁探索が終わったら、即Createを止める
std::cout << "--------------------------8----------------------------" << std::endl;
			//this->map.showMap2();
		}
	}

//2.障害物探索
	else if(this->search_flag == OBSTACLE) // 障害物探索
	{

		int goal;
		std::vector<Coordinate> move_point_list;  // Createが辿る座標を格納したリスト
		std::vector<Coordinate> tmp_create_list;
		std::vector<Coordinate> tmp_obstacle_list;

		bool Bumper = false;

// 2-1.createの現在座標を取得
		create_coord = this->create.getCurrentCoordinate();
// 2-2.createの現在座標から向かうメッシュを計算し、辿る座標リストを得る
		goal = this->block.getNextMeshNum();
		move_point_list = this->block.getMovePointList(create_coord, this->create.direction, goal);// → Createの現在座標を元に、ゴールまでに辿る座標のリストを得る
// 2-3.座標リスト通りに進む
		for(int i=0;i<move_point_list.size();i++)
		{
			// 2-3-1.次の座標値を渡して、移動し、各座表値の計算を行う
			this->create.runNextPoint(move_point_list[i], Bumper, tmp_create_list, tmp_obstacle_list);
			if(Bumper)	// runNextPoint()中に障害物に衝突したら
			{
				std::vector<Coordinate> SOC_list;	// search obstacle create list 障害物を探索する時のcreateの座標値リスト
				Bumper = false;
				tmp_obstacle_list = create.searchObstacle(SOC_list);	// 衝突した障害物の周りを回る
				for(int i=0;i<tmp_obstacle_list.size();i++)
				{
					this->map.push_back_ObstaclePointList( tmp_obstacle_list[i] );
					this->block.setMeshMark(tmp_obstacle_list[i], true);
					//obstacle_list.push_back(tmp_obstacle_list[i]);
				}
				for(int i=0;i<SOC_list.size();i++)
				{
					this->map.push_back_CreatePointList( SOC_list[i] );
					this->block.setMeshMark(SOC_list[i], false);// 
					//create_list.push_back(create_coord[i]);
				}
		}
			// 2-3-2 移動後の座標値を渡して、メッシュを更新
			for(int j=0;j<tmp_create_list.size();j++)
			{
				this->block.setMeshMark(tmp_create_list[j], false);// 
			// 2-3-3各座表の情報を渡して、map,obstacleリストを更新
				this->map.push_back_CreatePointList( tmp_create_list[j] );
			}
			for(int j=0;j<tmp_obstacle_list.size();j++)
			{
//				this->block.setMeshMark(tmp_obstacle_list[j], true);// 
				this->map.push_back_ObstaclePointList( tmp_obstacle_list[j] );
			}

		}
// 2-4.全部のメッシュをチェックし終えたかを判定
		if( !this->block.checkAllSearchEnd() )
		{
			this->search_flag = DOCK;

		}
	}

// 3.ドッキングステーションへゴール
	else if(this->search_flag == DOCK)
	{
		//else if(this->create.isDockFound())
		//{
		//	this->create.stopRun();// 壁探索が終わったら、即Createを止める
		//	this->search_flag = DOCK;			
		//}
		this->create.stopRun();
		runDemo(DEMO_COVER_AND_DOCK);
		this->finished = true;
		std::cout << "finished!" << std::endl;
	}



}


