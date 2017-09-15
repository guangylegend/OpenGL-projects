#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include "fish.h"
/*const int N=40;//��ͼ�����귶Χ1-N
const int M=40;//��ͼ�����귶Χ1-M
const int MAX_PLAYER=40;//������������ID��1-MAX_PLAYER
const int MAX_FOOD=50;//ˢ��ʳ������
const int FOOD_ROUND=5;//ˢ��ʳ��غ���
const int EMPTY=0;//����Ϊ��
const int FOOD=-1;//ʳ��
const int LEVEL_POINT=3;//�����ܻ�õĵ���*/
class st19:public fish{
	//Parameter list
	static const int iEvolveLevel1 = 4;
	static const int iEvolveLevel2 = 21;
	//static const int

	//variable list
	int aMap[N+2][M+2];//��ͼ��Ϣ
	int iTurn;//�غ���
	int iTargetX, iTargetY;//�ƶ�Ŀ��
	double dMoveValue;//�ƶ������ļ�ֵ,��ʼΪ0��Ȼ��Ϊ��õľ���+��õ�����ֵ
	int iAttackX, iAttackY;//�������
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

//�������������躯�����躯�������������豣֤����ÿ��������������ɵ���

//��Ҫʹ��ȫ�ֱ�������

#endif // PLAYER_H_INCLUDED
