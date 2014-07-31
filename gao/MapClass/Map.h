#ifndef __OpenCV__Map__
#define __OpenCV__Map__

/*****************************************************************************
 ** Includes
 *****************************************************************************/
#include <iostream>
#include <math.h>
#include <vector>
#include <string>
#include <fstream>
#include <stdio.h>
#include "cv.h"
#include "highgui.h"
#include <time.h>
#include "Coordinate.h"


/*****************************************************************************
 ** Define
 *****************************************************************************/
#define MAX_COORD_SIZE 29.4 //[m] block lenghが0.35[m]なので合計84ブロックになる
//ransac用 -------------------------------
#define SEARCH 500 	// 直線サーチ回数
#define THRESHOLD 5  // 最小二乗の閾値
//----------------------------------------


/*****************************************************************************
 ** Structure
 *****************************************************************************/
//txtファイルから読み込むの為の構造体
struct Location
{
	int number;
    double X;
    double Y;
};

/*****************************************************************************
 ** Classes
 *****************************************************************************/
class Map
{
public:
	// コンストラクタ 座標とブロック数を初期化
	
    //メソッド
    void showMap();     //マップを描画する

    // set method
	// createの座標リストをプッシュバック
	void push_back_CreatePointList(Coordinate coord)
	{
		this->create_point_list.push_back( coord );
	}

	// 障害物の座標リストをプッシュバック
	void push_back_ObstaclePointList( Coordinate coord )
	{
		this->obstacle_point_list.push_back(coord);
	}

    // get method
	float getCurrentCreatePoint_X()
	{
		Coordinate coord;
		std::vector<Coordinate>::iterator iterator;
		iterator = this->create_point_list.end();
		coord = *iterator;
		return coord.x;
	}
	float getCurrentCreatePoint_Y()
	{
		Coordinate coord;
		std::vector<Coordinate>::iterator iterator;
		iterator = this->create_point_list.end();
		coord = *iterator;
		return coord.y;
	}

	// create listの配列数を取得
	int getCreateListSize()
	{
		return this->create_point_list.size();
	}
	// obstacle list の配列数を取得
	int getObstacleListSize()
	{
		return this->obstacle_point_list.size();
	}

std::vector<Coordinate> getCreatePointList()
{
	return this->create_point_list;
}

std::vector<Coordinate> getObstaclePointList()
{
	return this->obstacle_point_list;
}


private:
    //変数
    //  Coordinate max;
	std::vector<Coordinate> create_point_list;			// create座標リスト
	std::vector<Coordinate> obstacle_point_list;	// （マップ生成用）障害物座標リスト
    CvPoint p1,p2,p3,p4;        //各直線の交点
    
    //メソッド
    void gauss(Coordinate coord1,Coordinate coord2, float &A, float &B, float &C, bool &xflag);     //２点の座標(x,y),(x',y')から直線の式を算出する
    void RANSAC_y_Ax_B(std::vector<Coordinate> wall, float &RANSAC_A, float &RANSAC_B, float &RANSAC_C, std::vector<Coordinate> &outliers, std::vector<Coordinate> &inliers, bool &xflag);     //座標のリストを基に直線の係数を算出する
    void line(double a,double b);       //各直線の交点の位置関係を求める
};

#endif
