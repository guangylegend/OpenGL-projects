#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include "fish.h"
/*const int N=40;//地图横坐标范围1-N
const int M=40;//地图纵坐标范围1-M
const int MAX_PLAYER=40;//最多玩家数量，ID从1-MAX_PLAYER
const int MAX_FOOD=50;//刷新食物数量
const int FOOD_ROUND=5;//刷新食物回合数
const int EMPTY=0;//格子为空
const int FOOD=-1;//食物
const int LEVEL_POINT=3;//升级能获得的点数*/
class st19:public fish{
	//Parameter list
	static const int iEvolveLevel1 = 4;
	static const int iEvolveLevel2 = 21;
	//static const int

	//variable list
	int aMap[N+2][M+2];//地图信息
	int iTurn;//回合数
	int iTargetX, iTargetY;//移动目标
	double dMoveValue;//移动产生的价值,初始为0，然后为获得的经验+获得的生命值
	int iAttackX, iAttackY;//定义变量
    int aSurviveTimes[100];

	//function list
	void refreshMap();
	int manDis(int, int, int, int);
	void refreshMove(int, int, int, int, double);
	double analysePosition(int, int);
	double analyseAttack(int argX, int argY);
	int m_abs(int x);
	int m_min(int x, int y);
	int m_max(int x, int y);
	int squre(int x);


    public:
		void init();

		void play();

		void revive(int& argX, int& argY);

};

//可自行增加所需函数所需函数及变量，但需保证上面每个函数的完整与可调用

//不要使用全局变量！！

#endif // PLAYER_H_INCLUDED
