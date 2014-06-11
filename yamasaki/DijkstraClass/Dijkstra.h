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

/*****************************************************************************
 ** Define
 *****************************************************************************/
#define		N			9       //ノード数
#define		START		0       //スタートノード
#define		COST_MAX	999     //仮無限値

/*****************************************************************************
 ** enum
 *****************************************************************************/
enum SearchMark
{
    UNSEARCH,   //未探索
    SEARCHED    //障害物
};

/*****************************************************************************
 ** Structure
 *****************************************************************************/
struct Mesh{
    int num;            //メッシュ番号
    int prev;           //直前のメッシュ番号
    SearchMark mark;    //探索済みか否か判断するマーカー
    int distance;       //スタートからそのノードに向かうまでの距離コスト
};

/*****************************************************************************
 ** Classes
 *****************************************************************************/
class Dijkstra{
public:
    //メソッド
    Mesh addFirstMesh(int, Mesh);    //メッシュの初期値をセットする
//    void readCost();    //メッシュ情報を格納する
    void searchRoute(); //経路探索を行う
    void showRoute();   //最短経路を表示する
    int init();        //初期化を行う
    Dijkstra();    //コンストラクタ
    Dijkstra(int);    //コンストラクタ
    
    //set Method
    //Blockクラスのコスト情報をセットする *p:Blockクラスのcost配列の先頭ポインタ
    void setCost(std::vector< std::vector<int> > &p)
    {
        for(int i=0; i<mesh_N; i++)
        {
            for(int j=0; j<mesh_N; j++)
            {
                cost[i][j] = p[i][j];
                std::cout << cost[i][j] << ",";
            }
            std::cout << std::endl;
        }
    }
    
private:
    //変数
    int next;   //次に向かうノード
    int min;    //現在の最短距離コスト
    int mesh_N; //メッシュ配列の要素数
    std::vector<Mesh> mesh;
    std::vector<Mesh> route;
    std::vector< std::vector<int> > cost;
    
};


#endif /* defined(__Dijkstra__Dijkstra__) */
