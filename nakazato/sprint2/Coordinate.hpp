/**
 * @file /kobuki_driver/src/test/Coordinate.cpp
 *
 * @Coordinate.
 **/
#pragma once
/*****************************************************************************
** Includes
*****************************************************************************/
#include <iostream>

/*****************************************************************************
** Enum
*****************************************************************************/
enum Mark
{
	OBSTACLE,
	WALL,
	DOCK,
};

/*****************************************************************************
** Classes
*****************************************************************************/

class Coordinate {
public:
  Coordinate(){
    this->X = 0;
    this->Y = 0;
  }
  Coordinate(float coord_x, float coord_y){
    this->X = coord_x;
    this->Y = coord_y;
  }

  void setCoordinate(float coord_x, float coord_y){
    this->X = coord_x;
    this->Y = coord_y;
  }
  float getX(){
    return this->X;
  }
  float getY(){
    return this->Y;
  }

  /***********************************
  **operation of Cordinate
  ************************************/
  bool operator<(const Coordinate &coord) const{
    return ( this->X < coord.X && this->Y < coord.Y);
  }

  bool operator>(const Coordinate &coord) const{
    return ( (this->X > coord.X) && (this->Y > coord.Y));
  }

  Coordinate & operator=(const Coordinate &coord){
    this->X = coord.X;
    this->Y = coord.Y;
    return *this;
  }
  
  Coordinate operator+(Coordinate &coord1){
    Coordinate coord2;
    coord2.X = this->X + coord1.X;
    coord2.X = this->X + coord1.X;
    return coord2;
  } 

  Coordinate &operator+=(const Coordinate &coord){
    this->X += coord.X;
    this->Y += coord.Y;
    return *this;
  }
  
  Coordinate operator-(Coordinate &coord1){
    Coordinate coord2;
    coord2.X = this->X - coord1.X;
    coord2.X = this->X - coord1.X;
    return coord2;
  } 

  Coordinate &operator-=(const Coordinate &coord){
    this->X -= coord.X;
    this->Y -= coord.Y;
    return *this;
  }

  bool operator==(const Coordinate &coord)const{
    return ((this->X == coord.X) && (this->Y == coord.Y));
  }  



  void setX(float x){
      this->X = x;
  }
  void setY(float y){
      this->Y = y;
  }

private:
  float X;
  float Y;
  enum Mark mark;
};

/*****************************************************************************
** Test Main
*****************************************************************************/
/*
int main() {
    Coordinate c;
    c.setCoordinate(2,5);
    std::cout << "Coord [" << c.getCoordinateX() << ", " << c.getCoordinateY() << "]" << std::endl;
    return 0;
}
*/
