#ifndef FISH_H
#define FISH_H
#include "mapblock.h"
#include "sys.h"


class fish {

private:
    int ID;
public:
    QString name;
    fish();
    ~fish();
    void setID(int);//����ID��ֻ�ɱ�����һ��
    bool move(int,int);//�ƶ���ĳ��λ��
    bool attack(int,int);//����ĳ��λ��
    int getPoint();//���ص�ǰ���ʣ�����
    int getLevel();//���ص�ǰ��ҵȼ�
    int getExp();//���ص�ǰ��Ҿ���
    int getX();//���ص�ǰ��Һ�����
    int getY();//���ص�ǰ���������
    int getHP();//���ص�ǰ�������ֵ
    int getMaxHP();//���ص�ǰ�����������
    int getAtt();//���ص�ǰ��ҹ�����
    int getSp();//���ص�ǰ����ٶ�
    int getID();//��ȡ���ID
    int askWhat(int,int);//���ص�ͼĳ��λ�����ݣ����ID��FOOD��EMPTY��
    int askHP(int);//ѯ��ĳ�����ID��ǰѪ��
    int getTotalPlayer();//��ȡ���������
    bool increaseHealth();//����health����1
    bool increaseStrength();//����Strength����1
    bool increaseSpeed();//����Speed����1

    virtual void init() = 0;//��ʼ��������ÿһ�����¿�ʼ������
    virtual void play() = 0;//�ж�������ÿ�غ��ж�������
    virtual void revive(int&,int&) = 0;//����ʱ���ã���������ѡ�񸴻�λ�ã������Ϸ��������
};

//�������������躯�������������豣֤����ÿ��������������ɵ���

//��Ҫʹ��ȫ�ֱ���!!!

#endif // FISH_H
