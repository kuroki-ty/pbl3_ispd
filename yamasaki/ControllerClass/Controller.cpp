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
    block.calcRoute(0.0, 0.0, UP, 1);
    route = block.getMovePointList();
    
    std::cout << "点" << std::endl;
	for(int i=0; i<route.size(); i++)
    {
        std::cout << route[i] << std::endl;
    }
    
    block.setMeshImpassable(660.0, 0.0);
    block.calcRoute(330.0, 0.0, UP, 3);
    route = block.getMovePointList();
    
    std::cout << "点" << std::endl;
	for(int i=0; i<route.size(); i++)
    {
        std::cout << route[i] << std::endl;
    }
    
    return 0;
}
