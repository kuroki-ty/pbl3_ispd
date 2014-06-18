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
    total_block_x = ceil(FIERD_X/block_x);  //横のメッシュ総数
    total_block_y = ceil(FIELD_Y/block_y);  //縦のメッシュ総数
    
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

/*vectorで動的配列を作成するためのメソッド
 構造体で配列を作っているため，内部のデータを個別に設定する必要がある*/
Mesh Block::addFirstBlock(int y, int x, int count, Mesh tmp)
{
    tmp.num = count;
    tmp.mark = UNKNOWN;
    tmp.center_x = x*block_x+block_x/2.0;
    tmp.center_y = y*block_y+block_y/2.0;
    
    return tmp;
}

//ノード間コストを決定するためのメソッド
void Block::addCost(int num, IRobotDirecton direction)
{
    current_mesh_num = num;                 //Createの現在位置のメッシュ番号
    current_create_direction = direction;   //Createの現在方向
    
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
        case UP:
            for(int i=current_mesh_num; (i%total_block_x)!=total_block_x-1; i++)
            {
                cost[i][i+1] = COST_LITTLE;
            }
            break;
        
        case DOWN:
            for(int i=current_mesh_num; (i%total_block_x)!=0; i--)
            {
                cost[i][i-1] = COST_LITTLE;
            }
            break;
            
        case RIGHT:
            for(int i=current_mesh_num; i>total_block_y-1; i-=total_block_y)
            {
                cost[i][i-total_block_x] = COST_LITTLE;
            }
            break;
            
        case LEFT:
            for(int i=current_mesh_num; i<total_block_x*total_block_y-total_block_x; i+=total_block_y)
            {
                cost[i][i+total_block_x] = COST_LITTLE;
            }
            break;
        
        default:
            break;
    }    
}

//ダイクストラ法で現在位置から目標位置までの最短経路を探索するメソッド
void Block::calcRoute(float x, float y, IRobotDirecton direction, int goal)
{
    int start;
    
    start = meshNumToCurrentPosition(x, y);
    addCost(start, direction);
    
    Dijkstra dijkstra(start, goal, total_block_x*total_block_y);
    dijkstra.setCost(cost);
    dijkstra.useDijkstra();
    route = dijkstra.getRoute();
}

//現在位置座標をメッシュ番号に変換するメソッド
int Block::meshNumToCurrentPosition(float x, float y)
{
    int node_num_x = 0;     //x軸のノード番号
    int node_num_y = 0;     //y軸のノード番号
    
    //x軸のノード番号を求める
    for(int i=0; i<total_block_x; i++)
    {
        if((x >= (float)(i*block_x)) && (x < (float)((i+1)*block_x)))
        {
            node_num_x = i;
            break;
        }
    }
    
    //y軸のノード番号を求める
    for(int i=0; i<total_block_y; i++)
    {
        if((y >= (float)(i*block_y)) && (y < (float)((i+1)*block_y)))
        {
            node_num_y = i;
            break;
        }
    }
    
    return ((node_num_y*total_block_x)+node_num_x);
}

/*****************************************************************************
 ** TestMain

int main(){
    Block block;
    block.calcRoute(0, 0, UP, 0, 8);
    
    return 0;
}
 
******************************************************************************/

