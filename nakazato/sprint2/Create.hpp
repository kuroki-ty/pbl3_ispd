#include <createoi.h>

#define VELOCITY 200		//直進速度[mm/s]
#define RADIUS_LEFT 230		//左回り回転半径[mm]
#define RADIUS_RIGHT 320	//右回り回転半径[mm]
#define WALL_DISTANCE_LOW 15.0	//壁とCreateの距離_低[cm]
#define WALL_DISTANCE_HIGH 20.0	//壁とCreateの距離_高[cm]


enum Direction
{
	PLUS_X,
	MINUS_X,
	PLUS_Y,
	MINUS_Y
};

enum State
{
    STOP,
    RUN,
    TURN,
	BUMPER
	
};



class Create
{
public:

void Run();
void changeDirection();

void setDirection(enum Direction d)
{
	this->direction = d;
}


private:
int present_direction;
float distance;
float velocity;
enum Direction direction;



};

#include "Create.cpp"
