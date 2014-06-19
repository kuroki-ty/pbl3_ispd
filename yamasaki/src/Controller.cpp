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
    
    std::vector<int> route;
    
    Block block;
    Coordinate current_position, obs_position;
    
    current_position.x = 0.0;
    current_position.y = 0.0;
    
    block.calcRoute(current_position, UP, 1);
    route = block.getMovePointList();
    
    std::cout << "点" << std::endl;
	for(int i=0; i<route.size(); i++)
    {
        std::cout << route[i] << std::endl;
    }
    
    obs_position.x = 660.0;
    obs_position.y = 0.0;
    current_position.x = 330.0;
    current_position.y = 0.0;
    
    block.setMeshMark(obs_position, IMPASSABLE);
    block.calcRoute(current_position, UP, 3);
    route = block.getMovePointList();
    
    std::cout << "点" << std::endl;
	for(int i=0; i<route.size(); i++)
    {
        std::cout << route[i] << std::endl;
    }
    
    return 0;
}
