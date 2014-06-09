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
#define		N			9
#define		START		0
#define		COST_MAX	999
#define     UNSEARCH    0
#define     SEARCHED    1

/*****************************************************************************
 ** Structure
 *****************************************************************************/
struct Mesh{
    int num;            //メッシュ番号
    int prev;           //直前のメッシュ番号
    int mark;           //探索済みか否か判断するマーカー
    int distance;       //スタートからそのノードに向かうまでの距離コスト
//    float center_x;     //メッシュの中心x座標
//    float center_y;     //メッシュの中心y座標
};

/*****************************************************************************
 ** Classes
 *****************************************************************************/
class Dijkstra{
public:
    //メソッド
    Mesh setMesh(int, Mesh);
    void readCost();    //メッシュ情報を格納する
    void searchRoute(); //経路探索を行う
    void showRoute();   //最短経路を表示する
    int init();        //初期化を行う
    Dijkstra();    //コンストラクタ
    
private:
    //変数
    int next;   //次に向かうノード
    int min;    //現在の最短距離コスト
    std::vector<Mesh> mesh;
    std::vector< std::vector<int> > cost;
    
};


#endif /* defined(__Dijkstra__Dijkstra__) */
