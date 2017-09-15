#include "fish.h"
#include "cstdlib"
#include "map"

#pragma once
/*
Author: ASEMBL
all rights reserved
*/
class st11:virtual public fish{

    private :
    
    int map[N+1][N+1] ;
    double value_to_move[N+1][N+1];
    int enemy_hp[1+MAX_PLAYER] , enemy_maxhp[1+MAX_PLAYER] , enemy_speed[1+MAX_PLAYER] , enemy_attk[1+MAX_PLAYER] ;
    int enemy_level[1+MAX_PLAYER] , enemy_coor[1+MAX_PLAYER][2] ;
    
    struct My_Information
    {
        int xcoor , ycoor;
        int attk , speed , maxhp , hp , exp , level , ID;
        int deadtime;
    }me;
    int counting ;  //  count how many numbers have been played
    int dir[4][2] ;
    
    void Map_Update();
    void MyInformation_Update();
    void MyPoint_assign();
    void EnemyInformation_Update();
    bool Necessary_hp();
    bool Necessary_speed();
    bool Necessary_attk();
    int Find_Food( int a = 0 );
    
    int abso( int x ){
        return x > 0 ? x : -x ;
    }
    bool inside( int x , int y ) {
        return x > 0 && x <= N && y > 0 && y <= M ;
    }
    template < class T > T max( T x , T y ) {
        return x > y ? x : y ;
    }
    void Value_Assess();
    void choose_position(int &, int &);
    void Attack_action();
    void Previous_Assess();
    
    public:
    void init();
    
    void play();
    
    void revive(int&,int&);
};

//可自行增加所需函数所需函数及变量，但需保证上面每个函数的完整与可调用

//不要使用全局变量！！
