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
#include "opencv/cv.h"
#include "opencv/highgui.h"
#include <time.h>


/*****************************************************************************
 ** Define
 *****************************************************************************/
#define MAX_COORD_SIZE 29.4 //[m] block lenghが0.35[m]なので合計84ブロックになる
#define LINE_NUM 4  //ライン数
//ransac用 -------------------------------
#define SEARCH 500 	// 直線サーチ回数
#define THRESHOLD 5  // 最小二乗の閾値
//----------------------------------------
#define pi 3.1415926


/*****************************************************************************
 ** Structure
 *****************************************************************************/
/*デバッグ用，競技プログラムでは使わない*/
//txtファイルから読み込むの為の構造体
struct Location
{
	int number;
    double X;
    double Y;
};
/**********/

/*****************************************************************************
 ** Classes
 *****************************************************************************/
class Map
{
public:
	// コンストラクタ
    Map();
	
    //メソッド
    void showMap();     //マップを描画する
    void showMap2();     //マップを描画する
    void calcLine();    //壁を示す直線を算出する
    void dividePoint(); // 障害物の余計な点を除去する

    // set method
	// createの座標リストをプッシュバック
	void push_back_CreatePointList(Coordinate coord)
	{
		this->create_point_list.push_back( coord );
	}
	// 壁の座標リストをプッシュバック
	void push_back_WallPointList( Coordinate coord )
	{
		this->wall_point_list.push_back(coord);
	}

	// 障害物の座標リストをプッシュバック
	void push_back_ObstaclePointList( Coordinate coord )
	{
		this->obstacle_point_list.push_back(coord);
	}
	// 障害物の座標リストをset テスト用
	void setObstaclePointList( std::vector<Coordinate> list )
	{
		this->obstacle_point_list.clear();
		this->obstacle_point_list = list;
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

    std::vector<Coordinate> getWallPointList()
    {
        return this->wall_point_list;
    }


    std::vector<Coordinate> getObstaclePointList()
    {
        return this->obstacle_point_list;
    }
    
    //線の交点座標を取得する
    std::vector<Coordinate> getIntersectionLine()
    {
        Coordinate tmp;
        std::vector<Coordinate> tmp_p;
        for(int i=0; i<p.size(); i++)
        {
            //縮尺を元に戻してpush_back
            tmp.x = p[i].x*10-500;
            tmp.y = p[i].y*10-500;
            tmp_p.push_back(tmp);
        }
        
        return tmp_p;
    }
    
    //直線の式の係数を取得する
    std::vector< std::vector<float> > getCoefficientLine(std::vector<bool> &xflag)
    {
        bool f = false;
        float a, b, c;
        std::vector<Coordinate> p;
        std::vector<float> list;
        std::vector< std::vector<float> > tmp;
        
        p = getIntersectionLine();      //直線の各交点をCoordinate型のpに格納
        
        for(int i=0; i<p.size(); i++)
        {
            if(i == p.size()-1)
            {
                gauss(p[i], p[0], a, b, c, f);
            }
            else
            {
                gauss(p[i], p[i+1], a, b, c, f);
            }
            xflag.push_back(f);
            list.push_back(a);
            list.push_back(b);
            list.push_back(c);
            tmp.push_back(list);
            list.clear();
        }
        
        return tmp;
    }


private:
    //変数
    //  Coordinate max;
	std::vector<Coordinate> create_point_list;			// create座標リスト
	std::vector<Coordinate> wall_point_list;			// wall座標リスト
	std::vector<Coordinate> obstacle_point_list;	// （マップ生成用）障害物座標リスト
    CvPoint p1,p2,p3,p4;        //各直線の交点
    std::vector<CvPoint> p;     //各直線の交点
    std::vector<float> RANSAC_A;
    std::vector<float> RANSAC_B;
    std::vector<float> RANSAC_C;
    std::vector<Coordinate> wall, outliers, inliers;
    std::vector< std::vector<Coordinate> > outliers_s, inliers_s;
    bool xflag;
    
    std::vector<Coordinate >obstacle_point_list2;
    std::vector<Coordinate >distance;
    std::vector<Coordinate> final;
    std::vector<Coordinate> group1;
    std::vector<Coordinate> group2;
    std::vector<Coordinate> group3;
    std::vector<Coordinate> group_a;
    //メソッド
    void gauss(Coordinate, Coordinate, float&, float&, float&, bool&);  //2点の座標(x,y),(x',y')から直線の式を算出する
    void line(double, double);       //各直線の交点の位置関係を求める
    void RANSAC_y_Ax_B(std::vector<Coordinate>, float&, float&, float&, std::vector<Coordinate>&, std::vector<Coordinate>&, bool&);     //座標のリストを基に直線の係数を算出する
};

#include "Map.cpp"
#endif
