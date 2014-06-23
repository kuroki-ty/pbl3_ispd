#include "main.hpp"



/*超音波センサの値を取得するプログラム*/

unsigned long int counter=0;

int main()
{
	// Createのスタート準備
	startOI("/dev/ttyUSB0");
	
	// Controler 宣言
	Controller controller;
	controller.create.run();
	// 超音波ON
//壁探索ループ
	int counter = 0;
	while(!controller.finished)	
	{
		controller.checkState();	// バンパーに当たったら回避モードに移行
//		std::cout << controller.create.getTotalAngle()  << std::endl;
	//std::cout << controller.create.getDistanceBySoner() <<std::endl;
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
