//
//  Block.h
//  Dijkstra
//
//  Created by yamasaki on 2014/06/09.
//  Copyright (c) 2014年 yamasaki. All rights reserved.
//

#ifndef __Dijkstra__Block__
#define __Dijkstra__Block__

/*****************************************************************************
 ** Includes
 *****************************************************************************/
#include <iostream>
#include <vector>
#include <math.h>

/*****************************************************************************
 ** Define
 *****************************************************************************/
#define     COST_LITTLE 1       //移動コスト小
#define     COST_BIG    2       //移動コスト大
#define		COST_MAX	999     //仮無限値
#define     FIERD_X     2000.0  //フィールドの横の長さ[mm](x座標)
#define     FIELD_Y     2000.0  //フィールドの縦の長さ[mm](y座標)
#define     IROBOT_D    330     //iRobotCreateの直径[mm] 330

/*****************************************************************************
 ** enum
 *****************************************************************************/
//フィールドのマーク情報
enum SearchMark
{
    UNKNOWN,   //未探索
    BLANK,     //空白(探索済)
    OBSTACLE   //障害物(探索済)
};

enum IRobotDirecton
{
    UP,     //+x軸方向
    DOWN,   //-x軸方向
    RIGHT,  //-y軸方向
    LEFT    //+y軸方向
};

/*****************************************************************************
 ** Structure
 *****************************************************************************/
struct Mesh{
    int num;            //メッシュ番号
    float center_x;     //メッシュの中心x座標
    float center_y;     //メッシュの中心y座標
    SearchMark mark;    //フィールドの探索情報を入れるマーカー
};

/*****************************************************************************
 ** Classes
 *****************************************************************************/
class Block{
public:
    //メソッド
    Block();   //コンストラクタ
    void calcRoute(int, int, IRobotDirecton, int);   //Dijkstraクラスをインスタンス生成し，ダイクストラ法でルートを計算させる
    
    //get method
    //最短経路の座標情報を返す
    std::vector<int> getRouteCoordinate()
    {
        return (route);
    }
    
private:
    //変数
    float block_x;  //メッシュの横の長さ[mm](x座標)
    float block_y;  //メッシュの縦の長さ[mm](y座標)
    int total_block_x;  //横のメッシュ数
    int total_block_y;  //縦のメッシュ数
    int current_mesh_num;   //Createの現在座標のメッシュ番号
    IRobotDirecton current_create_direction;    //現在のCreateの方向
    std::vector<int> route;     //Dijkstraクラスで計算したルートを格納するための配列
    std::vector< std::vector<int> > cost;    //メッシュ間の移動コスト
    std::vector< std::vector<Mesh> > block;  //メッシュ情報
    
    //メソッド
    Mesh addFirstBlock(int, int, int, Mesh);    //メッシュの初期値をセットする
    void addCost(int, IRobotDirecton);     //ノード間の重み付けを行う
    int meshNumToCurrentPosition(float, float);     //Createの現在座標からメッシュ番号に変換する
    
};

/*****************************************************************************
 ** Memo
 
・メッシュの軸について
 
        □□□□□□□□□□□□□□
        □□□□□□□□□□□□□□
        □□□□□□□□□□□□□□
        □□□□□□□□□□□□□□
  ↑     □□□□□□□□□□□□□□
  y,i   □□□□□□□□□□□□□□
        → x,j
 
 2次元配列　array[i][j] : array[y軸方向][x軸方向]　としてメッシュが表されている．
 また，メッシュ番号は左下(原点)を0として，+j方向に+1ずつ増加，
 例)
    □□□     →   [3][4][5]
    □□□     →   [0][1][2]
 
 ・ノード間のコスト
 コストを格納している配列について．配列の行番号，列番号はメッシュ番号を表す．
例)
 cost[0][1]:ノード番号0から1に向かう道のコスト
 cost[1][0]:ノード番号1から0に向かう道のコスト
 
 *****************************************************************************/




#endif /* defined(__Dijkstra__Block__) */
