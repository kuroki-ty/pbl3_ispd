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
    block.calcRoute(0, 0, UP, 40);
    route = block.getRouteCoordinate();
    
    std::cout << "点" << std::endl;
	for(int i=0; i<route.size(); i++)
    {
        std::cout << route[i] << std::endl;
    }
    
    return 0;
}
