/**
 * @file
 *
 * @Coordinate.
 **/
struct Coordinate
{
	float x;
	float y;
};

//Createの方向情報
enum IRobotDirecton
{
    UP,     //+x軸方向
    DOWN,   //-x軸方向
    RIGHT,  //-y軸方向
    LEFT    //+y軸方向
};
