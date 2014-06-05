

void Tracks::blockListInit()
{
    //block_list.clear();
    
    // ブロックの個数を計算
    int idx_x = MAX_COORD_SIZE/DEFAULT_BLOCK_LENGTH;
    int idx_y = MAX_COORD_SIZE/DEFAULT_BLOCK_LENGTH;

    std::cout << "Block num is ...." << std::endl;
    std::cout << "idx_x = " << idx_x << std::endl; 
    std::cout << "idx_y = " << idx_y << std::endl;
   
    // block のリスト作成
    // block_listの配列構成
    // □□□□□□□□□□□□□□・・・・ idx_x
    // □□□□□□□□□□□□□□・・・・ idx_x
    // □□□□□□□□□□□□□□・・・・ idx_x
    // ・
    // ・
    // idx_y

    // block_list[横x][縦y]

	if( idx_x>0 && idx_y>0 )
	{
		for(int i = 0; i<idx_x; i++)
		{
      
			std::vector<Block> list;
      
			for(int j = 0; j<idx_y; j++)
			{
				Coordinate coord;
				// DEFAULT_BLOCK_LENGTH(0.35) ピクセル（?）毎にブロックを作成していく
	        	coord.setCoordinate(i*DEFAULT_BLOCK_LENGTH-0.4f, j*DEFAULT_BLOCK_LENGTH-0.4f);
				Block block(coord, DEFAULT_BLOCK_LENGTH, i, j);
				list.push_back(block);
	        }

	        this->block_list.push_back(list);   
	        list.clear(); 
		}
	}
}

