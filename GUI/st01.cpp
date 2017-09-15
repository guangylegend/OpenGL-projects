#include "st01.h"
#include <cstdio>
#include <time.h>
#include <cstdlib>
void st01::init(){
    
    srand((unsigned)time(NULL));
    increaseStrength();
	
	for ( int i = 0 ; i < 50 ; ++i )
    {
		increaseHealth();
		increaseSpeed();
	}
}

void st01::play(){
    init();
    int x , y ;
	
	for ( int i = -1 ; i < 50 ; ++i )
        for ( int j = -1 ; j < 50 ; ++j )
        {
            if ( askWhat(i,j) > 0 )//���ص�ͼĳ��λ�����ݣ����ID��FOOD��EMPTY��
            {
                move(i+1,j);
                while ( attack(i,j) );

            }
        }
    for ( int i = -1 ; i < 50 ; ++i )
        askHP(i);//ѯ��ĳ�����ID��ǰѪ��
    
    for ( int i = 0 ; i < 50 ; ++i )
    {
        x = rand() ;
        y = rand() ;
        move(x,y);//�ƶ���ĳ��λ��
    }
    for ( int i = 0 ; i < 50 ; ++i )
    {
        x = rand() ;
        y = rand() ;
        attack(x,y);//�ƶ���ĳ��λ��
    }
    
    getLevel();//���ص�ǰ��ҵȼ�
    getExp();//���ص�ǰ��Ҿ���
    getX();//���ص�ǰ��Һ�����
    getY();//���ص�ǰ���������
    getHP();//���ص�ǰ�������ֵ
    getMaxHP();//���ص�ǰ�����������
    getAtt();//���ص�ǰ��ҹ�����
    getSp();//���ص�ǰ����ٶ�
    getID();//��ȡ���ID
    
}

void st01::revive(int& x,int& y){

    x = y = -1 ;    
}
