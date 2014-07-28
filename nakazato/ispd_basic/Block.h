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
#include "Coordinate.h"

/*****************************************************************************
 ** Define
 *****************************************************************************/
#define     COST_LITTLE 1       //移動コスト小
#define     COST_BIG    2       //移動コスト大
#define		COST_MAX	999     //仮無限値
#define     FIERD_X     1980.0  //フィールドの横の長さ[mm](x座標)
#define     FIELD_Y     1980.0  //フィールドの縦の長さ[mm](y座標)
#define     IROBOT_D    330     //iRobotCreateの直径[mm] 330

/*****************************************************************************
 ** enum
 *****************************************************************************/
//フィールドのマーク情報
enum SearchMark
{
    UNKNOWN,    //未探索
    PASSABLE,   //通れる
    IMPASSABLE  //通れない
};

/*****************************************************************************
 ** Structure
 *****************************************************************************/
struct Mesh{
    int num;            //メッシュ番号
    Coordinate center;     //メッシュの中心座標(x,y)
    SearchMark mark;    //フィールドの探索情報を入れるマーカー
};

struct Wall{
    int start;  //壁の始まり
    int end;    //壁の終わり
};

/*****************************************************************************
 ** Classes
 *****************************************************************************/
class Block{
public:
    //メソッド
    Block();   //コンストラクタ
    bool isStartMesh(Coordinate, Coordinate, int);   //壁探索時にスタート位置まで戻ってきたかどうかを判定する
    bool checkAllSearchEnd();   //全メッシュを探索し終えたかどうか判定する
    void showMesh();    //メッシュの探索状況を表示させる
    void fillMesh();    //壁探索終了後に壁の外のメッシュを全て探索済みにする
    
    //get method
    /*Dijkstraクラスをインスタンス生成し，ダイクストラ法でルートを計算させる
    　探索ルートの座標値をリストにして返す(長いので処理はBlock.cpp内に書く)*/
    std::vector<Coordinate> getMovePointList(Coordinate, IRobotDirection direction, int);
    //次に向かうノード番号を取得する
    int getNextMeshNum()
    {
        for(int i=0; i<total_block_y; i++)
        {
            if(i%2 == 0)
            {
                for(int j=0; j<total_block_x; j++)
                {
                    if(block[i][j].mark == UNKNOWN)
                    {
                        return (block[i][j].num);
                    }
                }
            }
            else
            {
                for(int j=total_block_x-1; j>=0; j--)
                {
                    if(block[i][j].mark == UNKNOWN)
                    {
                        return (block[i][j].num);
                    }
                }
            }
        }
        
        return (-1);       //例外処理，-1を返す
    }
    
    
    //set method
    //障害物があるメッシュにPASSABLE or IMPASSABLE情報をセットする
    void setMeshMark(Coordinate coord, bool bump)
    {
        int num;
        SearchMark mark;
        
        if(bump)
        {
            mark = IMPASSABLE;
        }
        else
        {
            mark = PASSABLE;
        }
        
        num = meshNumToCurrentPosition(coord);
        block[num/total_block_x][num%total_block_x].mark = mark;
    }
    
private:
    //変数
    float block_x;  //メッシュの横の長さ[mm](x座標)
    float block_y;  //メッシュの縦の長さ[mm](y座標)
    int total_block_x;  //横のメッシュ数
    int total_block_y;  //縦のメッシュ数
    int current_mesh_num;   //Createの現在座標のメッシュ番号
    IRobotDirection current_create_direction;    //現在のCreateの方向
    std::vector<int> route;     //Dijkstraクラスで計算したルートを格納するための配列
    std::vector< std::vector<int> > cost;    //メッシュ間の移動コスト
    std::vector< std::vector<Mesh> > block;  //メッシュ情報
    std::vector<Coordinate> movePointList;  //探索ルートの座標情報を格納するための配列
    
    //メソッド
    Mesh addFirstBlock(int, int, int, Mesh);    //メッシュの初期値をセットする
    void addCost(int, IRobotDirection);          //ノード間の重み付けを行う
    int meshNumToCurrentPosition(Coordinate);     //Createの現在座標からメッシュ番号に変換する
    std::vector<Coordinate> routeCoordinateToRouteMeshNum(std::vector<int>);  //ルートのメッシュ番号を座標値に変換する

};

#include "Block.cpp"

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
