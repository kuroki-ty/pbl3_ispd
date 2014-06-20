#include "main.h"



/*超音波センサの値を取得するプログラム*/

unsigned long int counter=0;

int main()
{
	// Createのスタート準備
	startOI("/dev/ttyUSB0");
	
	// Controler 宣言
	Controller controller;
	controller.create.run();
	int counter = 0;
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
	controller.output_ObstacleList();

	return 0;
}
