//
//  Controller.cpp
//  Dijkstra
//
//  Created by yamasaki on 2014/06/12.
//  Copyright (c) 2014年 yamasaki. All rights reserved.
//

#include "Controller.h"
#include "Block.h"

int main(){
    
    std::vector<Coordinate> moveList;
    
    Block block;
    Coordinate current_position, obs_position;
    
    current_position.x = 0.0;
    current_position.y = 0.0;
    
    obs_position.x = 150.0;
    obs_position.y = 150.0;
    block.setMeshMark(obs_position, true);
    
    block.setMeshMark(current_position, false);
    
    while(block.checkAllSearchEnd())
    {
        moveList = block.getMovePointList(current_position, UP, block.getNextMeshNum());
        
        for(int i=0; i<moveList.size(); i++)
        {
            current_position.x = moveList[i].x;
            current_position.y = moveList[i].y;
            
            block.setMeshMark(current_position, false);
        }
        
        block.showMesh();
    }
    
    
    
    
/*
    current_position.x = 0.0;
    current_position.y = 0.0;
    
    moveList = block.getMovePointList(current_position, UP, 1);
    
    std::cout << "探索ルート" << std::endl;
	for(int i=0; i<moveList.size(); i++)
    {
        std::cout << "(" << moveList[i].x << "," << moveList[i].y << ")" << std::endl;
    }
    
    block.showMesh();
    
    obs_position.x = 250.0;
    obs_position.y = 50.0;
    current_position.x = 150.0;
    current_position.y = 50.0;
    
    block.setMeshMark(obs_position, IMPASSABLE);
    moveList = block.getMovePointList(current_position, UP, 8);
    
    std::cout << "探索ルート" << std::endl;
	for(int i=0; i<moveList.size(); i++)
    {
        std::cout << "(" << moveList[i].x << "," << moveList[i].y << ")" << std::endl;
    }
    
    block.showMesh();
 */
    
    return 0;
}
