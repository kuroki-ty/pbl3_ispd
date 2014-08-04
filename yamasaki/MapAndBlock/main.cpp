//
//  main.cpp
//  opencv_test
//
//  Created by IspdUnit on 2014/07/31.
//  Copyright (c) 2014年 yamasaki. All rights reserved.
//

#include "main.h"

int main(){
    Map map;
    Block block;
    std::vector<Coordinate> p;
    std::vector<bool> xflag;
    std::vector< std::vector<float> > ransac;
    
    map.calcLine();
    
    p = map.getIntersectionLine();
    ransac = map.getCoefficientLine(xflag);
    
    block.setMeshMarks(p, ransac, xflag);
    
    block.showMesh();
    block.fillMesh();
    block.showMesh();

    /*
    std::cout << "線の交点" << std::endl;
    for(int i=0; i<p.size(); i++)
    {
        std::cout << p[i].x << ", " << p[i].y << std::endl;
    }
    std::cout << "線の係数" << std::endl;
    for(int i=0; i<ransac.size(); i++)
    {
        std::cout << "線" << i+1 << std::endl;
        for(int j=0; j<ransac[i].size(); j++)
        {
            std::cout << ransac[i][j] << ", ";
        }
        std::cout << std::endl;
        std::cout << xflag[i] << std::endl;
    }

    map.showMap();
     */
}
