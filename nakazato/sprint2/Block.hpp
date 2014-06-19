/**
 * @file /kobuki_driver/src/test/Block.cpp
 *
 * @Coordinate.
 **/
#pragma once
/*****************************************************************************
** Includes
*****************************************************************************/
#include <iostream>
#include "Coordinate.h"
/*****************************************************************************
** Define
*****************************************************************************/
#define DEFAULT_BLOCK_LENGTH 0.35
#define DEFAULT_COORD_X 0
#define DEFAULT_COORD_Y 0

enum SearchMark
{
	UNKNOWN,	// 未探索
	PASSABLE,   // 通れる
	IMPASSABLE	// 通れない
};


enum IRMark	// 赤外線センサの有無のマーク
{
	NOT_EXIST,
	EXIST
};

/*****************************************************************************
** Structs
*****************************************************************************/

struct Borders
{
	float left;
	float right;
	float up;
	float down;

  Borders(){ left = right = up = down = 0.0; }
};

/*****************************************************************************
** Classes
*****************************************************************************/

class Block {

//Public member
public:
//Constructors
  Block(){
    this->centerPoint.setCoordinate(DEFAULT_COORD_X, DEFAULT_COORD_Y);
    this->block_length = DEFAULT_BLOCK_LENGTH;
    this->searchmark = UNKNOWN;
    this->irmark = NOT_EXIST;
  }
  Block(double coord_x, double coord_y, double length, int tag_x, int tag_y){
    this->centerPoint.setCoordinate(coord_x, coord_y);
    this->block_length = length;
    this->searchmark = UNKNOWN;
    this->irmark = NOT_EXIST;
  }
  Block(Coordinate coord, double length, int tag_x, int tag_y){
    this->centerPoint = coord;
    this->block_length = length;
    this->searchmark = UNKNOWN;
    this->irmark = NOT_EXIST;
  }

  bool isUnkonwn(){
    return (this->searchmark==UNKNOWN?true:false);
  }
  bool isBlank(){
    return (this->searchmark==PASSABLE?true:false);
  }
  bool isObstacle(){
    return (this->searchmark==IMPASSABLE?true:false);
  }

  float getCenterPointX(){
    return this->centerPoint.getX();
  }
  float getCenterPointY(){
    return this->centerPoint.getY();
  }

//Get methods
  Coordinate getCenterPoint(){
    return this->centerPoint;
  }

  float getBlockLength(){
    return this->block_length;
  }
  enum SearchMark getSerchMark(){
    return this->searchmark;
  }
  enum IRMark getIRMark(){
    return this->irmark;
  }

//Set methods
  void setMark(enum SearchMark m){
    this->searchmark = m;
    return;
  }
  void setIRMark(enum IRMark ir){
    this->irmark = ir;
    return;
  }

  void setCenterPoint(Coordinate coord){
    this->centerPoint = coord;
    return;
  }


//Private member
private:
	float block_length;			// ブロックの大きさ
	Coordinate centerPoint;		// ブロックの中心の座標
	enum SearchMark searchmark;	// 探索用マーク
	enum IRMark irmark;			// 赤外線検出マーク

};

