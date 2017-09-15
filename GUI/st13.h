#ifndef ST13_H
#define ST13_H
#include "fish.h"

class st13:public fish{
private:
	int cond1[N+1][M+1],cond2[N+1][M+1];//�ֱ��¼��һ�ֺ���һ�ֵĵ�ͼ
	int density1[N+1][M+1];//��¼���и����Χ�����ʳ��ķḻ�̶�
	int allspeed[MAX_PLAYER+1];//��¼�²�����ĸ�����ٶ�ֵ
	int allhp1[MAX_PLAYER+1],allhp2[MAX_PLAYER+1];//��¼��һ�ֺ���һ�ָ���ҵ�����ֵ
	int allspot1[MAX_PLAYER+1][3],allspot2[MAX_PLAYER+1][3];//��¼��һ�ֺ���һ�ָ���ҵ�λ��
	int allspot3[MAX_FOOD+1][3];//��¼��һ������ʳ���λ��
	int pnum;//�������
	bool justrevive;//�Ƿ�ոո���
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
	int hpincrease();//��ʳ�����ܻ�õ�����ֵ����
	int expneed();//�������辭����
	int px,py,pspeed,pattack,php,fnum;
	void setPoint(int,int,int);
public:
    st13(){}
	void init();
    void play();
    void revive(int&,int&);
};

#endif
