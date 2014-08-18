//
//  Block.cpp
//  Dijkstra
//
//  Created by yamasaki on 2014/06/09.
//  Copyright (c) 2014年 yamasaki. All rights reserved.
//

#include "Block.h"
#include "Dijkstra.h"

/*****************************************************************************
 ** Construtor
 *****************************************************************************/
Block::Block()
{
    block_x = IROBOT_D;     //メッシュ一個分の横の長さ
    block_y = IROBOT_D;     //メッシュ一個分の縦の長さ
    total_block_x = 2*ceil(FIERD_X/block_x);  //横のメッシュ総数(余白を含む)
    total_block_y = 2*ceil(FIELD_Y/block_y);  //縦のメッシュ総数(余白を含む)
    
    //ノード間移動コスト配列の初期設定
    std::vector<int> list_c;
    for(int i=0; i<total_block_x*total_block_y; i++)
    {
        for(int j=0; j<total_block_x*total_block_y; j++)
        {
            list_c.push_back(COST_MAX);
        }
        cost.push_back(list_c);
        list_c.clear();
    }
    
    //メッシュ配列の初期設定
    std::vector<Mesh> list_b;
    int count = 0;
    for(int i=0; i<total_block_y; i++)
    {
        for(int j=0; j<total_block_x; j++)
        {
            Mesh tmp;
            list_b.push_back(addFirstBlock(i, j, count, tmp));
            count++;
        }
        block.push_back(list_b);
        list_b.clear();
    }
    
    //ルート配列の初期設定
    for(int i=0; i<total_block_x*total_block_y; i++)
    {
        route.push_back(0);
    }
}

/*****************************************************************************
 ** Method
 *****************************************************************************/

//壁探索時にCreateがスタートノードまで戻ってきたかどうか判定するメソッド
bool Block::isStartMesh(Coordinate start, Coordinate current_pos, int distance)
{
    int start_num;          //スタートノード番号
    int current_pos_num;    //現在位置のノード番号
    
    //座標値をノード番号に変換
    start_num = meshNumToCurrentPosition(start);
    current_pos_num = meshNumToCurrentPosition(current_pos);
    
    if(current_pos_num - total_block_x -1 <= 0 && distance > total_block_x*block_x)
    {
        return (true);     //スタートノードに戻ってきた場合はtrue
    }
    else
        return (false);    //スタートノードに戻ってない場合はfalse
}

//全メッシュを探索し終えたかどうか判定する
bool Block::checkAllSearchEnd()
{
    for(int i=0; i<total_block_y; i++)
    {
        for(int j=0; j<total_block_x; j++)
        {
            if(block[i][j].mark == UNKNOWN)
            {
                return (true);
            }
        }
    }
    
    return (false);
}

//現在のメッシュ探索状況を表示するメソッド
void Block::showMesh()
{
    std::cout << "/********現在のメッシュ探索状況********/" << std::endl;
    
    for(int i=total_block_y-1; i>=0; i--)
    {
        for(int j=0; j<total_block_x; j++)
        {
            if(block[i][j].mark == UNKNOWN)
            {
                std::cout << "△ ";
            }
            else if(block[i][j].mark == PASSABLE)
            {
                std::cout << "○ ";
            }
            else
            {
                std::cout << "× ";
            }
        }
        std::cout << std::endl;
    }
    
    std::cout << "/*********************************/" << std::endl;
    
}

//壁探索終了後に壁の外のメッシュを全て探索済みにする
void Block::fillMesh()
{
    std::vector<Wall> wall;     //壁の始まりと終わりを行ごとに記録するための配列
    Wall tmp;
    for(int i=0; i<total_block_y; i++)
    {
        tmp.start = -1;
        tmp.end = -1;
        wall.push_back(tmp);    //初期値は0
    }
    
    /*横方向に壁の外のメッシュを埋める*/
    for(int i=0; i<total_block_y; i++)
    {
        for(int j=0; j<total_block_x; j++)
        {
            if(block[i][j].mark == IMPASSABLE)
            {
                if(wall[i].start == -1){    //壁の始まりは初期値だった時のみ記録
                    wall[i].start = j;
                }
                wall[i].end = j;            //壁の終わりは毎回記録
            }
        }
    }
    
    //壁の外にあるメッシュをIMPASSABLEにする
    for(int i=0; i<total_block_y; i++)
    {
        for(int j=0; j<total_block_x; j++)
        {
            if((j<wall[i].start) || (j>wall[i].end))
            {
                block[i][j].mark = IMPASSABLE;
            }
        }
    }
    /*****************************/
}


//ダイクストラ法で現在位置から目標位置までの最短経路を探索するメソッド
std::vector<Coordinate> Block::getMovePointList(Coordinate coord, IRobotDirection direction, int goal)
{
    int start;
    
    start = meshNumToCurrentPosition(coord);
    addCost(start, direction);
    
    Dijkstra dijkstra(start, goal, total_block_x*total_block_y);
    dijkstra.setCost(cost);
    dijkstra.useDijkstra();
    route = dijkstra.getRoute();
    
    movePointList = routeCoordinateToRouteMeshNum(route);
    
    return (movePointList);
}

//ダイクストラ法で現在位置からDockまでの最短経路を探索するメソッド
std::vector<Coordinate> Block::getMovePointListToDock(Coordinate coord, IRobotDirection direction)
{
    int start, goal = 0;
    int goalset_flag = 0;
    
    //startは現在座標
    start = meshNumToCurrentPosition(coord);
    addCost(start, direction);

    //goalは赤外線の値が強いメッシュ
    for(int i=0; i<total_block_y; i++)
    {
        for(int j=0; j<total_block_y; j++)
        {
            if((block[i][j].ir_value == 254) || (block[i][j].ir_value == 246) || (block[i][j].ir_value == 250) || (block[i][j].ir_value == 252))
            {
                goal = block[i][j].num;
                goalset_flag = 1;
                std::cout << "ドッキングステーションが見つかりました" << std::endl; 
                std::cout << "ノード番号：" << goal << "へ向かいます" << std::endl; 
                break;
            }
        }
    }
    if(goalset_flag == 0)
    {
        for(int i=0; i<total_block_y; i++)
        {
            for(int j=0; j<total_block_y; j++)
            {
                if((block[i][j].ir_value == 244) || (block[i][j].ir_value == 248))
                {
                    goal = block[i][j].num;
                    std::cout << "ドッキングステーションが見つかりました" << std::endl; 
                    std::cout << "ノード番号：" << goal << "へ向かいます" << std::endl;
                    break;
                }
            }
        }
    }
    
    Dijkstra dijkstra(start, goal, total_block_x*total_block_y);
    dijkstra.setCost(cost);
    dijkstra.useDijkstra();
    route = dijkstra.getRoute();
    
    movePointList = routeCoordinateToRouteMeshNum(route);
    
    return (movePointList);
}


void Block::setMeshMarks(std::vector<Coordinate> &p, std::vector< std::vector<float> > &ransac, std::vector<bool> &flag)
{
    int resolution = 100;
    float dx;
    std::vector<float> x;
    Coordinate coord;
    
    for(int i=0; i<p.size(); i++)
    {
        if(i == p.size())
        {
            dx = (p[i].x-p[0].x)/resolution;
            for(int k=0; k < resolution; k++)
            {
                x.push_back(p[i].x+k*dx);
            }
        }
        else
        {
            dx = (p[i+1].x-p[i].x)/resolution;
            for(int k=0; k < resolution; k++)
            {
                x.push_back(p[i].x+k*dx);
            }
        }
        
        for(int j=0; j<x.size(); j++)
        {
            
            coord.x = x[j];
            
            if(flag[i])
            {
                coord.y = ransac[i][2];     //y=c
            }
            else
            {
                coord.y = ransac[i][0] * coord.x + ransac[i][1];  //y=ax+b
            }
            
            setMeshMark(coord, true);
        }
        
        x.clear();
    }
}

/*vectorで動的配列を作成するためのメソッド
 構造体で配列を作っているため，内部のデータを個別に設定する必要がある*/
Mesh Block::addFirstBlock(int y, int x, int count, Mesh tmp)
{
    tmp.num = count;
    tmp.mark = UNKNOWN;
    tmp.center.x = x*block_x+block_x/2.0 - 10.0*block_x;     //余白を2*block_x取る
    tmp.center.y = y*block_y+block_y/2.0 - 10.0*block_y;     //余白を2*block_y取る
    tmp.ir_value = 0;

    return tmp;
}

//ノード間コストを決定するためのメソッド
void Block::addCost(int num, IRobotDirection direction)
{
    current_mesh_num = num;                 //Createの現在位置のメッシュ番号
    current_create_direction = direction;   //Createの現在方向
    
    //コスト配列を初期化する
    std::vector<int> list_c;
    for(int i=0; i<total_block_x*total_block_y; i++)
    {
        for(int j=0; j<total_block_x*total_block_y; j++)
        {
            list_c.push_back(COST_MAX);
        }
        cost.push_back(list_c);
        list_c.clear();
    }
    
    //隣り合うノード間はCOST_BIG，斜めに位置するノード間はCOST_MAX
    for(int i=0; i<total_block_x*total_block_y; i++)
    {
        for(int j=0; j<total_block_x*total_block_y; j++)
        {
            if((i==j+1) || (j==i+1) || (i==j+total_block_x) || (j==i+total_block_x))
            {
                cost[i][j] = COST_BIG;
                if(((i%total_block_x)==total_block_x-1) && (j==i+1))
                {
                    cost[i][j] = COST_MAX;
                }
                if(((j%total_block_x)==total_block_x-1) && (i==j+1))
                {
                    cost[i][j] = COST_MAX;
                }
            }
        }
    }
    
    //Createの向いている方向は回転なしで移動できるため，移動コストを最小にする
    switch(current_create_direction){
        case PLUS_X:
            for(int i=current_mesh_num; (i%total_block_x)!=total_block_x-1; i++)
            {
                cost[i][i+1] = COST_LITTLE;
            }
            break;
        
        case MINUS_X:
            for(int i=current_mesh_num; (i%total_block_x)!=0; i--)
            {
                cost[i][i-1] = COST_LITTLE;
            }
            break;
            
        case MINUS_Y:
            for(int i=current_mesh_num; i>total_block_y-1; i-=total_block_y)
            {
                cost[i][i-total_block_x] = COST_LITTLE;
            }
            break;
            
        case PLUS_Y:
            for(int i=current_mesh_num; i<total_block_x*total_block_y-total_block_x; i+=total_block_y)
            {
                cost[i][i+total_block_x] = COST_LITTLE;
            }
            break;
        
        default:
            break;
    }
    
    //障害物があるノードの上下左右のコストをCOST_MAXにする
    for(int i=0; i<total_block_y; i++)
    {
        for(int j=0; j<total_block_x; j++)
        {
            if(block[i][j].mark == IMPASSABLE)
            {
                //メッシュ最下列時の処理
                if(block[i][j].num < total_block_x)
                {
                    //メッシュ番号が0の時の処理
                    if(block[i][j].num == 0)
                    {
                        //通行不可ノード→周辺ノードのコストを更新
                        cost[block[i][j].num][block[i][j].num+total_block_x] = COST_MAX;    //上
                        cost[block[i][j].num][block[i][j].num+1] = COST_MAX;    //右
                        //周辺ノード→通行不可ノードのコストを更新
                        cost[block[i][j].num+total_block_x][block[i][j].num] = COST_MAX;    //上
                        cost[block[i][j].num+1][block[i][j].num] = COST_MAX;    //右
                    }
                    else
                    {
                        //通行不可ノード→周辺ノードのコストを更新
                        cost[block[i][j].num][block[i][j].num+total_block_x] = COST_MAX;    //上
                        cost[block[i][j].num][block[i][j].num+1] = COST_MAX;    //右
                        cost[block[i][j].num][block[i][j].num-1] = COST_MAX;    //左
                        //周辺ノード→通行不可ノードのコストを更新
                        cost[block[i][j].num+total_block_x][block[i][j].num] = COST_MAX;    //上
                        cost[block[i][j].num+1][block[i][j].num] = COST_MAX;    //右
                        cost[block[i][j].num-1][block[i][j].num] = COST_MAX;    //左
                    }
                }
                //メッシュ最上列時の処理
                else if(block[i][j].num >= total_block_x*total_block_y-total_block_x)
                {
                    //メッシュ番号が最大の時の処理
                    if(block[i][j].num == total_block_x*total_block_y-1)
                    {
                        //通行不可ノード→周辺ノードのコストを更新
                        cost[block[i][j].num][block[i][j].num-total_block_x] = COST_MAX;    //下
                        cost[block[i][j].num][block[i][j].num-1] = COST_MAX;    //左
                        //周辺ノード→通行不可ノードのコストを更新
                        cost[block[i][j].num-total_block_x][block[i][j].num] = COST_MAX;    //下
                        cost[block[i][j].num-1][block[i][j].num] = COST_MAX;    //左
                    }
                    else
                    {
                        //通行不可ノード→周辺ノードのコストを更新
                        cost[block[i][j].num][block[i][j].num-total_block_x] = COST_MAX;    //下
                        cost[block[i][j].num][block[i][j].num+1] = COST_MAX;    //右
                        cost[block[i][j].num][block[i][j].num-1] = COST_MAX;    //左
                        //周辺ノード→通行不可ノードのコストを更新
                        cost[block[i][j].num-total_block_x][block[i][j].num] = COST_MAX;    //下
                        cost[block[i][j].num+1][block[i][j].num] = COST_MAX;    //右
                        cost[block[i][j].num-1][block[i][j].num] = COST_MAX;    //左
                    }
                }
                //その他メッシュ列時の処理
                else
                {
                    //通行不可ノード→周辺ノードのコストを更新
                    cost[block[i][j].num][block[i][j].num+total_block_x] = COST_MAX;    //上
                    cost[block[i][j].num][block[i][j].num-total_block_x] = COST_MAX;    //下
                    cost[block[i][j].num][block[i][j].num+1] = COST_MAX;    //右
                    cost[block[i][j].num][block[i][j].num-1] = COST_MAX;    //左
                    //周辺ノード→通行不可ノードのコストを更新
                    cost[block[i][j].num+total_block_x][block[i][j].num] = COST_MAX;    //上
                    cost[block[i][j].num-total_block_x][block[i][j].num] = COST_MAX;    //下
                    cost[block[i][j].num+1][block[i][j].num] = COST_MAX;    //右
                    cost[block[i][j].num-1][block[i][j].num] = COST_MAX;    //左
                }
            }
        }
    }
}

//現在位置座標をメッシュ番号に変換するメソッド
int Block::meshNumToCurrentPosition(Coordinate coord)
{
    int node_num_x = 0;     //x軸のノード番号
    int node_num_y = 0;     //y軸のノード番号
    
    //x軸のノード番号を求める
    for(int i=0; i<total_block_x; i++)
    {
        if((coord.x >= (float)(i*block_x-10.0*block_x)) && (coord.x < (float)((i+1)*block_x-10.0*block_x)))
        {
            node_num_x = i;
            break;
        }
    }
    
    //y軸のノード番号を求める
    for(int i=0; i<total_block_y; i++)
    {
        if((coord.y >= (float)(i*block_y-10.0*block_y)) && (coord.y < (float)((i+1)*block_y-10.0*block_y)))
        {
            node_num_y = i;
            break;
        }
    }
    return ((node_num_y*total_block_x)+node_num_x);
}

//ルートのメッシュ番号を座標値に変換するメソッド
std::vector<Coordinate> Block::routeCoordinateToRouteMeshNum(std::vector<int> route_mesh_num){
    
    std::vector<Coordinate> route_coord;
    
    for(int i=0; i<route_mesh_num.size(); i++)
    {
        route_coord.push_back(block[route_mesh_num[i]/total_block_x][route_mesh_num[i]%total_block_x].center);
    }
    
    return (route_coord);
}

/*****************************************************************************
 ** TestMain

int main(){
    Block block;
    
    block.showMesh();
    
    return 0;
}
 
*****************************************************************************/

