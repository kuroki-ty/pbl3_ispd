/**
 * @file
 *
 * @Coordinate.
 **/

#ifndef __Coordinate__
#define __Coordinate__

struct Coordinate
{
	float x;
	float y;

	Coordinate()
	{
		x=0.0;
		y=0.0;
	}
};

//Createの方向情報
enum IRobotDirection
{
    PLUS_X,     //+x軸方向
    MINUS_X,   //-x軸方向
    PLUS_Y,    //+y軸方向
    MINUS_Y  //-y軸方向
};

#endif
