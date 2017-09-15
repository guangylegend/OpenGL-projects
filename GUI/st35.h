#include "fish.h"
#include "cstdlib"
#pragma once
const int Dead = -1;
const int Active = 1;
const int Revive = 0;

struct character//����
{
    int HP;//HP
    int guessSpeed;//�ٶȹ���
    int guessLevel;//�ȼ�����
    int guessMaxHP;//���HP����
    int guessAtt;
    int Status;//״̬(Dead, Active, Revive)
    int X, Y;//����(��Ҫ����Ϊ��¼ǰһ�������Թ����ٶ�)
    bool Flag;//ɨ���ʾ
};


class st35:public fish{
    private:

    int Map[M + 2][N + 2];//ս�����
    int ValueMap[M + 2][N + 2];//��λ���۱�
    character Fishes[N + 2];

    int MyID;//�ҵ�ID
    int MyX, MyY;//�ҵ�����
    int MyHP, MySp, MyAtt, MyMaxHP;//�ҵ�����
    int MyLv, MyExp, MaxExp;//�ҵĵȼ�������ֵ����һ�ȼ���������
    int LeftPoint;//�ҵ�ʣ�����

    int Round;//�غ���

    int Status;

    bool incStr(int);//��������n��
    bool incHth(int);//����Ѫ��n��
    bool incSpd(int);//�����ٶ�n��
    int Distance(int, int, int ,int);

    void ScanBattlefield();//ɨ��ս��

    int Condition();

    void Play_when_Weak();//����״̬�²���
    void Evaluate_when_Weak();//����״̬�µĹ��ۺ���
    void Play_when_Normal();//����״̬�µĲ���
    void Evaluate_when_Normal();//����״̬�µĹ��ۺ���
    void Play_when_Frenzy();//���״̬�µĲ���
    void Evaluate_when_Frenzy();//���״̬�µĹ��ۺ���
    void Play_when_Dying();
    void Evaluate_when_Dying();
    void Evaluate_when_Revive();

    void FindBest(int&, int&, int, int);

    void Level_Up(int);

    int distance(int, int, int, int);

    public:
    void init();

    void play();

    void revive(int&,int&);

};


//�������������躯�����躯�������������豣֤����ÿ��������������ɵ���

//��Ҫʹ��ȫ�ֱ�������


/*��revive��play��ͬʱ��round�����һ��Flag��ʾ����playǰ�Ƿ������revive
������һ�ֺ󸴻���Թ���������Speed
���ǰ����Ϣ��¼
����������Ϣ��¼
Dying״̬������Ҫ�޸�
����
Level_Up()��ûд
����level mod 3 = 0ʱ��Att��������ٶȣ�ÿ�ξ���Health
*/
