

/*
void state(enum State state)
{

	if(event == STOP)
	{
		this->create.runWithSoner();
	}
	else if(event == RUN)
	{

	}
	else if(event == TURN)
	{

	}
	else if(event == BUMPER)
	{

	}

}
*/

void Controller::checkBumper()
{

	if(	getBumpsAndWheelDrops() == 2)
	{	
		this->create.changeDirection();	// バンパーに当たったので方向転換モードに移行
	}
}


