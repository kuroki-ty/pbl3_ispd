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

Dijkstra::Dijkstra(int s, int g, int m_N){
    start = s;
    goal = g;
    mesh_N = m_N;
    
    for(int i=0; i<mesh_N; i++)
    {
        Mesh tmp;
        mesh.push_back(addFirstMesh(i, tmp));
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

void Dijkstra::searchRoute()
{
	mesh[start].distance = 0;
	next = start;
	
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

void Dijkstra::writeRoute()
{
    std::vector<int> tmp;
    for(int i=0; i<mesh_N; i++)
    {
        tmp.push_back(0);
    }
    
    int count = 0;
    tmp[count] = mesh[goal].num;
    do
    {
        tmp[count+1] = mesh[tmp[count]].prev;
        count++;
    }while(tmp[count-1] != start);
    
    for(int i=0; i<count; i++)
    {
        route.push_back(0);
    }

	for(int i=0; i<count; i++)
    {
        route[i] = tmp[count-i-1];
    }
}

void Dijkstra::useDijkstra()
{
    searchRoute();
    writeRoute();
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