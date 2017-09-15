#pragma once
#ifndef ST16_H_INCLUDED
#define ST16_H_INCLUDED
#include "fish.h"
#include "cstdlib"

class st16: public fish{

	struct point{
		int x;
		int y;
	};

    public:

    int circle;

	int deadtime;

    void init();

    void play();

    void revive(int&,int&);



    private:

	int x;

	int y;

	int lastHP;

    struct point killAble[100];

    bool reach[N+1][M+1];

    void getReach();

    void getmap();

    int countEnemy(int x,int y,int speed);//输出以speed能攻击到（x，y）点的玩家个数

    int countKillAble(int x,int y,int att,struct point save[]);//输出攻击范围内能一次击杀的玩家数，并记录在数组中

	int countFood(int x,int y,int speed,struct point save[]);//输出攻击范围内的食物，并记录在数组中

	bool killLowHpPlayer(int number,struct point save[]);//对最安全的玩家补刀

	bool killLowHpPlayerH(int number,struct point save[]);//对可杀血最多的玩家补刀

	bool killFood(int number,struct point save[]);//攻击最安全的食物

	bool defend(int att);//对给自己攻击的敌人展开可行的反击

	bool move2safe();//到达安全区域

	bool move2food();

	bool move2kill();//到达杀戮区域

	bool runFar();

	bool reachAble(int x,int y,int speed);//给出能否到达（x，y）点;

};


#endif // ST16_H_INCLUDED
