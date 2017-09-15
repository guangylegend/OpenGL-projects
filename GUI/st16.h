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

    int countEnemy(int x,int y,int speed);//�����speed�ܹ�������x��y�������Ҹ���

    int countKillAble(int x,int y,int att,struct point save[]);//���������Χ����һ�λ�ɱ�������������¼��������

	int countFood(int x,int y,int speed,struct point save[]);//���������Χ�ڵ�ʳ�����¼��������

	bool killLowHpPlayer(int number,struct point save[]);//���ȫ����Ҳ���

	bool killLowHpPlayerH(int number,struct point save[]);//�Կ�ɱѪ������Ҳ���

	bool killFood(int number,struct point save[]);//�����ȫ��ʳ��

	bool defend(int att);//�Ը��Լ������ĵ���չ�����еķ���

	bool move2safe();//���ﰲȫ����

	bool move2food();

	bool move2kill();//����ɱ¾����

	bool runFar();

	bool reachAble(int x,int y,int speed);//�����ܷ񵽴x��y����;

};


#endif // ST16_H_INCLUDED
