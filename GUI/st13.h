#ifndef ST13_H
#define ST13_H
#include "fish.h"

class st13:public fish{
private:
	int cond1[N+1][M+1],cond2[N+1][M+1];//分别记录上一局和这一局的地图
	int density1[N+1][M+1];//记录所有格点周围的鱼和食物的丰富程度
	int allspeed[MAX_PLAYER+1];//记录猜测出来的各玩家速度值
	int allhp1[MAX_PLAYER+1],allhp2[MAX_PLAYER+1];//记录上一局和这一局各玩家的生命值
	int allspot1[MAX_PLAYER+1][3],allspot2[MAX_PLAYER+1][3];//记录上一局和这一局各玩家的位置
	int allspot3[MAX_FOOD+1][3];//记录这一局所有食物的位置
	int pnum;//玩家数量
	bool justrevive;//是否刚刚复活
	static int step[5][3];
	void getMap();
	void getallhp();
	void newspeed();
	void getden1();
	void trans1();
	void trans2();
	void trans3();
	double value(int,int);
	int istherefood(int,int);
	int istherefish(int,int);
	int hpincrease();//吃食物所能获得的生命值增长
	int expneed();//升级所需经验数
	int px,py,pspeed,pattack,php,fnum;
	void setPoint(int,int,int);
public:
    st13(){}
	void init();
    void play();
    void revive(int&,int&);
};

#endif
