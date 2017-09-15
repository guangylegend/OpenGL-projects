#include "fish.h"
#include <cstring>
#include <time.h>
#include <cstdio>
#include <algorithm>
#include <cstdlib>
#pragma once

class st08:virtual public fish{
	private:
	int map[41][41],Player_Speed[41],last_location[41][2],value[41][41],maxHP[41];
	int Exp[41],myExp;
	int appeal[41],appealtimes[41],found;
	int deadtimes,myx,myy;
	int myHP,myMaxHP,myLevel;
	int dir[4][2];
	int Rounds,relivex,relivey,myID,mySpeed,myAttack;
	int max_value,maxx,maxy;
	void increasePoint();
	void increaseRandPoint(int x,int y,int z);
	int calc_format();
	int calc_MaxHP(int x);
	void Ready();
	void EatFood();
	void AttackFish();
	void FindFood();
	int SecondStepFood(int x,int y);
	void DecHP();
	void Naughty();
	int NearToMe(int x,int y);

	public:
    void init();
    void play();
    void revive(int&,int&);

};

//可自行增加所需函数所需函数及变量，但需保证上面每个函数的完整与可调用

//不要使用全局变量！！
