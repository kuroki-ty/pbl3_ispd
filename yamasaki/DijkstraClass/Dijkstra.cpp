//
//  Dijkstra.cpp
//  Dijkstra
//
//  Created by yamasaki on 2014/06/09.
//  Copyright (c) 2014年 yamasaki. All rights reserved.
//

#include "Dijkstra.h"

/*****************************************************************************
 ** Construtor
 *****************************************************************************/
Dijkstra::Dijkstra(){
    for(int i=0; i<N; i++)
    {
        Mesh tmp;
        mesh.push_back(addFirstMesh(i, tmp));
        route.push_back(addFirstMesh(i, tmp));
    }
    
    std::vector<int> list;
    for(int i=0; i<N; i++)
    {
        for(int j=0; j<N; j++)
        {
            list.push_back(COST_MAX);
        }
        cost.push_back(list);
        list.clear();
    }
}

Dijkstra::Dijkstra(int m_N){
    mesh_N = m_N;
    
    for(int i=0; i<mesh_N; i++)
    {
        Mesh tmp;
        mesh.push_back(addFirstMesh(i, tmp));
        route.push_back(addFirstMesh(i, tmp));
    }
    
    std::vector<int> list;
    for(int i=0; i<mesh_N; i++)
    {
        for(int j=0; j<mesh_N; j++)
        {
            list.push_back(COST_MAX);
        }
        cost.push_back(list);
        list.clear();
    }
}

/*****************************************************************************
 ** Method
 *****************************************************************************/
int Dijkstra::init()
{
    return 0;
}
    
Mesh Dijkstra::addFirstMesh(int num, Mesh tmp)
{
    tmp.num = num;
    tmp.prev = init();
    tmp.mark = UNSEARCH;
    tmp.distance = COST_MAX;
    
    return tmp;
}

/*
void Dijkstra::readCost()
{
	cost[0][1] = 1;
	cost[1][0] = 2;
	cost[1][2] = 2;
	cost[2][1] = 1;
	cost[3][4] = 2;
	cost[4][3] = 3;
	cost[4][5] = 1;
	cost[5][4] = 3;
	cost[6][7] = 2;
	cost[7][6] = 1;
	cost[7][8] = 1;
	cost[8][7] = 2;
	cost[0][3] = 1;
	cost[3][0] = 2;
	cost[3][6] = 2;
	cost[6][3] = 1;
	cost[1][4] = 3;
	cost[4][1] = 1;
	cost[4][7] = 4;
	cost[7][4] = 1;
	cost[2][5] = 2;
	cost[5][2] = 2;
	cost[5][8] = 7;
	cost[8][5] = 1;
}
*/

void Dijkstra::searchRoute()
{
	mesh[START].distance = 0;
	next = START;
	
    int k;  //iの代用
    do
	{
		k = next;
		mesh[k].mark = SEARCHED;
		min = COST_MAX;
		for(int j=0; j<mesh_N; j++)
		{
			if(mesh[j].mark)
            {
                continue;
            }
			if((cost[k][j] < COST_MAX) && (mesh[k].distance + cost[k][j] < mesh[j].distance))
			{
				mesh[j].distance = mesh[k].distance + cost[k][j];
				mesh[j].prev = k;
			}
			if(mesh[j].distance < min)
			{
				min = mesh[j].distance;
				next = j;
			}
		}
	}while(min < COST_MAX);
}

void Dijkstra::showRoute()
{
    int count = 0;
    route[count] = mesh[mesh_N-1];
    do
    {
        route[count+1] = mesh[route[count].prev];
        count++;
    }while(route[count-1].num != 0);
    
    std::cout << "点   直前の点   最短距離" << std::endl;
	for(int i=count-1; i>=0; i--)
    {
        std::cout << route[i].num << "       " << route[i].prev << "      " << route[i].distance << std::endl;
    }
}

/*****************************************************************************
 ** TestMain
 
int main(void)
{
    Dijkstra dijkstra;
    
    dijkstra.readCost();
    dijkstra.searchRoute();
    dijkstra.showRoute();
    
    return 0;
}

*****************************************************************************/