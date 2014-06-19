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
Dijkstra::Dijkstra(int s, int g, int m_N)
{
    start = s;      //スタートノード決定
    goal = g;       //ゴールノード決定
    mesh_N = m_N;   //メッシュサイズ決定
    
    //探索後のノード情報を格納する配列の初期設定
    for(int i=0; i<mesh_N; i++)
    {
        DijkstraMesh tmp;
        mesh.push_back(addFirstMesh(i, tmp));
    }
    
    //ノードコストを格納する配列の初期設定
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

/*vectorで動的配列を作成するためのメソッド
 構造体で配列を作っているため，内部のデータを個別に設定する必要がある*/
DijkstraMesh Dijkstra::addFirstMesh(int num, DijkstraMesh tmp)
{
    tmp.num = num;
    tmp.prev = init();
    tmp.mark = UNSEARCH;
    tmp.distance = COST_MAX;
    
    return tmp;
}

//初期化メソッド
int Dijkstra::init()
{
    return 0;
}

//ダイクストラ法でルートを探索するメソッド
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

/*メッシュ配列に格納されている経路情報から最短ルートのみを抽出するためのメソッド
 最短経路はroute配列に格納される*/
void Dijkstra::writeRoute()
{
    int tmp;                        //ルートノードを一時格納する変数
    int pri_tmp = mesh[goal].num;   //直前ノードを一時格納する変数
    route.push_back(pri_tmp);
    
    //ルートノードがstartノードになるまでループ
    while(tmp != start)
    {
        tmp = mesh[pri_tmp].prev;
        pri_tmp = tmp;
        route.push_back(tmp);
    }

    
    std::reverse(route.begin(), route.end());   //route配列の順序入れ替え
    
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