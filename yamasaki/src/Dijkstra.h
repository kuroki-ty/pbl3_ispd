//
//  Dijkstra.h
//  Dijkstra
//
//  Created by yamasaki on 2014/06/09.
//  Copyright (c) 2014年 yamasaki. All rights reserved.
//

#ifndef __Dijkstra__Dijkstra__
#define __Dijkstra__Dijkstra__

/*****************************************************************************
 ** Includes
 *****************************************************************************/
#include <iostream>
#include <vector>
#include <algorithm>

/*****************************************************************************
 ** Define
 *****************************************************************************/
#define		COST_MAX	999     //仮無限値

/*****************************************************************************
 ** enum
 *****************************************************************************/
enum DijkstraSearchMark
{
    UNSEARCH,   //未探索
    SEARCHED    //障害物
};

/*****************************************************************************
 ** Structure
 *****************************************************************************/
struct DijkstraMesh{
    int num;            //メッシュ番号
    int prev;           //直前のメッシュ番号
    int distance;       //スタートからそのノードに向かうまでの距離コスト
    DijkstraSearchMark mark;    //探索済みか否か判断するマーカー
};

/*****************************************************************************
 ** Classes
 *****************************************************************************/
class Dijkstra{
public:
    //メソッド
    Dijkstra(int, int, int);    //コンストラクタ
    void useDijkstra();     //ダイクストラ法を使う
    
    //get method
    std::vector<int> getRoute()
    {
        return (route);
    }
    
    //set Method
    //Blockクラスのコスト情報をセットする &p:Blockクラスのcost配列の先頭ポインタ
    void setCost(std::vector< std::vector<int> > &p)
    {
        for(int i=0; i<mesh_N; i++)
        {
            for(int j=0; j<mesh_N; j++)
            {
                cost[i][j] = p[i][j];
            }
        }
    }
    
private:
    //変数
    int start;  //スタートノード
    int goal;   //ゴールノード
    int next;   //次に向かうノード
    int min;    //現在の最短距離コスト
    int mesh_N; //メッシュ配列の要素数
    std::vector<DijkstraMesh> mesh;         //メッシュ格納用配列
    std::vector<int> route;                 //最短ルート格納用配列
    std::vector< std::vector<int> > cost;   //ノードコスト格納用配列
    
    //メソッド
    DijkstraMesh addFirstMesh(int, DijkstraMesh);    //メッシュの初期値をセットする
    int init();        //初期化を行う
    void searchRoute(); //経路探索を行う
    void writeRoute();   //最短経路をrouteに書き込む
    
};


#endif /* defined(__Dijkstra__Dijkstra__) */
