#ifndef SYS_H
#define SYS_H

class sys {
    public:
    sys();
    ~sys();
    bool move(int,int,int);//�ƶ���ĳ��λ��
    bool attack(int,int,int);//����ĳ��λ��
    int getPoint(int);//���ص�ǰ���ʣ�����
    int getLevel(int);//���ص�ǰ��ҵȼ�
    int getExp(int);//���ص�ǰ��Ҿ���
    int getX(int);//���ص�ǰ��Һ�����
    int getY(int);//���ص�ǰ���������
    int getHP(int);//���ص�ǰ�������ֵ
    int getMaxHP(int);//���ص�ǰ�����������
    int getAtt(int);//���ص�ǰ��ҹ�����
    int getSp(int);//���ص�ǰ����ٶ�
    int askWhat(int,int);//���ص�ͼĳ��λ�����ݣ����ID��FOOD��EMPTY��
    int askHP(int);//ѯ��ĳ�����ID��ǰѪ��
    int getTotalPlayer();//��ȡ���������
    bool increaseHealth(int);//����health����1
    bool increaseStrength(int);//����Strength����1
    bool increaseSpeed(int);//����Speed����1

};

#endif // SYS_H
