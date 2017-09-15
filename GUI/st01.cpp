#include "st01.h"
#include <cstdio>
#include <time.h>
#include <cstdlib>
void st01::init(){
    
    srand((unsigned)time(NULL));
    increaseStrength();
	
	for ( int i = 0 ; i < 50 ; ++i )
    {
		increaseHealth();
		increaseSpeed();
	}
}

void st01::play(){
    init();
    int x , y ;
	
	for ( int i = -1 ; i < 50 ; ++i )
        for ( int j = -1 ; j < 50 ; ++j )
        {
            if ( askWhat(i,j) > 0 )//返回地图某个位置内容（玩家ID或FOOD或EMPTY）
            {
                move(i+1,j);
                while ( attack(i,j) );

            }
        }
    for ( int i = -1 ; i < 50 ; ++i )
        askHP(i);//询问某个玩家ID当前血量
    
    for ( int i = 0 ; i < 50 ; ++i )
    {
        x = rand() ;
        y = rand() ;
        move(x,y);//移动到某个位置
    }
    for ( int i = 0 ; i < 50 ; ++i )
    {
        x = rand() ;
        y = rand() ;
        attack(x,y);//移动到某个位置
    }
    
    getLevel();//返回当前玩家等级
    getExp();//返回当前玩家经验
    getX();//返回当前玩家横坐标
    getY();//返回当前玩家纵坐标
    getHP();//返回当前玩家生命值
    getMaxHP();//返回当前玩家生命上限
    getAtt();//返回当前玩家攻击力
    getSp();//返回当前玩家速度
    getID();//获取玩家ID
    
}

void st01::revive(int& x,int& y){

    x = y = -1 ;    
}
