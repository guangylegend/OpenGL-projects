/*const int N=40;//��ͼ�����귶Χ1-N
const int M=40;//��ͼ�����귶Χ1-M
const int MAX_PLAYER=40;//������������ID��1-m_max_PLAYER
const int MAX_FOOD=50;//ˢ��ʳ������
const int FOOD_ROUND=5;//ˢ��ʳ��غ���
const int EMPTY=0;//����Ϊ��
const int FOOD=-1;//ʳ��
const int LEVEL_POINT=3;//�����ܻ�õĵ���*/

/**
�汾��v1.3
˵����
ѡ��Ĳ��ԣ�
	1���ӵ����
	2����ȫ����
	3���������
δѡ��Ĳ��ԣ�
	1��������������
	2������ͳ��
	3�����㼼��
*/
#include"st19.h"
#include<queue>
#include<cmath>

static const int aDirection[5][3] = {{0,0,0}, {0,0,1}, {0,1,0}, {0,-1,0}, {0,0,-1}};//�����ƶ��ķ���
static const int dMax = 12321232;
using namespace std;

/** @brief revive
  *
  * @todo: ����
  */
void st19::revive(int& argX, int& argY)
{
    for (int i = 1; i < 41; i++)
        if (askHP(i) > 0) aSurviveTimes[i]++;
	iTurn ++;//��������ʧȥ��һ�غ�
	argX = N;
	argY = M;
}

int st19::m_abs(int argX)//��ѧ����
{
	if (argX < 0) return -argX;
	return argX;
}

int st19::m_min(int argX, int argY)
{
	if (argX < argY) return argX;
	return argY;
}

int st19::m_max(int argX, int argY)
{
	if (argX > argY) return argX;
	return argY;
}

int st19::squre(int argX)
{
	return argX * argX;
}

int st19::manDis(int argX1, int argY1, int argX2, int argY2)
//�����پ���
{
	return m_abs(argX1 - argX2) + m_abs(argY1 - argY2);
}

void st19::refreshMove(int argX, int argY, int argAttX, int argAttY, double argValue)
//�����ж�����
{
	if (argValue > dMoveValue) {
		iTargetX = argX;
		iTargetY = argY;
		iAttackX = argAttX;
		iAttackY = argAttY;
		dMoveValue = argValue;
	}
}

double st19::analysePosition(int argX, int argY)
{
	if (getLevel() > 21) return 0;
	double dPotentialValue = 0;
	double dMaxValue = 0;
	for (int i = 1; i <= N; i++)
		for (int j = 1; j <= M; j++)
			if (aMap[i][j] != getID()){//�ų��Լ�
				if ((manDis(argX, argY, i, j) <= getSp()+1)) {//��һ�����Թ�������
					dPotentialValue += analyseAttack(i, j);
				}
				dMaxValue += analyseAttack(i,j);
			}
	dPotentialValue = dPotentialValue / dMaxValue;

	for (int i = 1; i <= N; i++)//����ܵ�����Σ�ն�
		for (int j = 1; j <= M; j++){
			if (aMap[i][j] != getID()){ //�ų��Լ�
				if (aMap[i][j] > 0){
					if (manDis(argX, argY, i, j) <= 5)
						if (dPotentialValue > 0)
                            dPotentialValue *= (sqrt(0.0+getHP()/getMaxHP()) * 0.99);//��������������Σ�ն�Ϊ1
						else
							dPotentialValue -= 1/100;
				}
			}
		}

	return dPotentialValue;
}
double st19::analyseAttack(int argX, int argY){//TODO:������������
	double dTempValue = 0;
	if (getLevel() <= iEvolveLevel2)
	{
		if (aMap[argX][argY] == FOOD) {//����ˮ�ݻ�õļ�ֵ
			dTempValue += (1 + m_min(getMaxHP(), getHP()+m_max(2,getMaxHP()/10)) - getHP());
			//ˮ�ݵļ�ֵΪ1������) + ���Իָ�������ֵ
		}

		if (aMap[argX][argY] > 0) {//�������õļ�ֵ
			int iKillHp = askHP(aMap[argX][argY]);
			if (getAtt() >= iKillHp)
				dTempValue += m_max(1, getLevel() / 2) + iKillHp/getAtt();
				//ɱ�˵ļ�ֵΪ���飬ͬ��������ɱѪ������
		}
	}
	else
	{
        /*if (aMap[argX][argY] == FOOD) {//����ˮ�ݻ�õļ�ֵ
			dTempValue += 1;//ֻ���Ǿ���
        }*/

		if (aMap[argX][argY] > 0) {//�������õļ�ֵ
			int iKillHp = askHP(aMap[argX][argY]);
			if (getAtt() >= iKillHp)
				dTempValue += iKillHp + 1/*��֤ɱ�˶����ǳ�ˮ��*/;
            else
                dTempValue += iKillHp / 100;
                //dTempValue += aSurviveTimes[aMap[argX][argY]] / 500;
		}
	}
	return dTempValue;
}

void st19::play()
{
	//��ʼ��
	iTurn ++;//���ӻغ���
	refreshMap();//���µ�ͼ
    for (int i = 1; i <= N; i++)
        for (int j = 1; j <= M; j++)
            aSurviveTimes[aMap[i][j]]++;
	//����
	iTargetX = getX(), iTargetY = getY();//�ƶ�Ŀ��
	dMoveValue = -dMax;//�ƶ������ļ�ֵ,��ʼΪ0��Ȼ��Ϊ��õľ���+��õ�����ֵ
	iAttackX = 0, iAttackY = 0;
	for (int i = 1; i <= N; i++)
		for (int j = 1; j <= M; j++)
			if ((manDis(getX(), getY(), i, j) <= getSp()) && (aMap[i][j] == EMPTY)){
				double dPositionValue = analysePosition(i, j);//�ڳ�ˮ�ݻ���ɱ�˵ļ�ֵͬ��ʱ��λ�ú���������
				refreshMove(i, j, 0, 0, dPositionValue);
				for (int iDir = 1; iDir <= 4; iDir++){
					refreshMove(i, j, i + aDirection[iDir][1], j + aDirection[iDir][2], dPositionValue + analyseAttack(i + aDirection[iDir][1], j + aDirection[iDir][2]));//����
				}
			}


	//�ж�
	move(iTargetX, iTargetY);
	if (iAttackX != 0) attack(iAttackX, iAttackY);


	//�����������ʱ�ӵ㣩
	if (getPoint() > 0) {
		if (getLevel() <= iEvolveLevel1){
			increaseHealth();
			increaseSpeed();
			increaseSpeed();
		}
		else
			if (getLevel() <= iEvolveLevel2){
				increaseHealth();
				increaseSpeed();
				increaseStrength();
			}
			else{
				increaseSpeed();
				increaseStrength();
				if (getSp() - getAtt() < 10) increaseSpeed();
					else increaseStrength();
			}
	}

}

/** @brief init
  *
  * @todo: document this function
  */
void st19::init()
{
	//�����ʼ��������ͷΪ������
	const int iHealth = 4;
	const int iSpeed = 5;
	const int iStrength = 1;
	for (int i = 1; i <= iHealth; i++) increaseHealth();
	for (int i = 1; i <= iSpeed; i++) increaseSpeed();
	for (int i = 1; i <= iStrength; i++) increaseStrength();


	//��ʼ��������
	iTurn = 0;
	for (int i = 0; i <= N+1; i++)//���ñ߽�Ϊ0
		for (int j = 0; j <= M+1; j++)
			aMap[i][j] = 0;
    for (int i = 0; i < 100; i++)
        aSurviveTimes[i] = 0;

}

/** @brief (one liner)
  *
  * ���µ�ͼ
  */
void st19::refreshMap()
//���µ�ͼ
{
	for (int i = 1; i <= N; i++)
		for (int j = 1; j <= M; j++)
			aMap[i][j] = askWhat(i,j);

}

