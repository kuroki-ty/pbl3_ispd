#include "main.h"



/*超音波センサの値を取得するプログラム*/

unsigned long int counter=0;

int main()
{
	// Createのスタート準備
	startOI("/dev/ttyUSB0");
	// Controler 宣言
	Controller controller;
	controller.create.init();
//	controller.create.run();
	int counter = 0;
	//waitTime(2.0);

	int wait=0;
	while(1)
	{
		waitTime(0.2);
		std::cout << controller.create.getDistanceBySoner() << std::endl;
		if(wait>20)
		{
			break;
		}
		wait++;
	}

	while(!controller.finished)	
	{
		controller.checkState();	// バンパーに当たったら回避モードに移行
	}

std::cout << "count:" << counter << std::endl;
std::cout << "map_list_size" << controller.map.getCreateListSize() << std::endl;
std::cout << "obstacle_list_size" << controller.map.getObstacleListSize() << std::endl;
std::cout << "distance" << controller.create.getTotalDistance() << std::endl;
	
	// create終了
	stopOI();

	// 記録した座標値を出力
	controller.output_CreateList();
	controller.output_WallList();
	controller.output_ObstacleList();

	return 0;
}
