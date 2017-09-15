/*const int N=40;//地图横坐标范围1-N
const int M=40;//地图纵坐标范围1-M
const int MAX_PLAYER=40;//最多玩家数量，ID从1-m_max_PLAYER
const int MAX_FOOD=50;//刷新食物数量
const int FOOD_ROUND=5;//刷新食物回合数
const int EMPTY=0;//格子为空
const int FOOD=-1;//食物
const int LEVEL_POINT=3;//升级能获得的点数*/

/**
版本：v1.3
说明：
选择的策略：
	1、加点策略
	2、安全估计
	3、收益估计
未选择的策略：
	1、经验期望计算
	2、死亡统计
	3、留点技术
*/
#include"st19.h"
#include<queue>
#include<cmath>

static const int aDirection[5][3] = {{0,0,0}, {0,0,1}, {0,1,0}, {0,-1,0}, {0,0,-1}};//可以移动的方向
static const int dMax = 12321232;
using namespace std;

/** @brief revive
  *
  * @todo: 复活
  */
void st19::revive(int& argX, int& argY)
{
    for (int i = 1; i < 41; i++)
        if (askHP(i) > 0) aSurviveTimes[i]++;
	iTurn ++;//死亡导致失去了一回合
	argX = N;
	argY = M;
}

int st19::m_abs(int argX)//数学函数
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
//曼哈顿距离
{
	return m_abs(argX1 - argX2) + m_abs(argY1 - argY2);
}

void st19::refreshMove(int argX, int argY, int argAttX, int argAttY, double argValue)
//更新行动策略
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
			if (aMap[i][j] != getID()){//排除自己
				if ((manDis(argX, argY, i, j) <= getSp()+1)) {//下一步可以攻击到的
					dPotentialValue += analyseAttack(i, j);
				}
				dMaxValue += analyseAttack(i,j);
			}
	dPotentialValue = dPotentialValue / dMaxValue;

	for (int i = 1; i <= N; i++)//求出总的期望危险度
		for (int j = 1; j <= M; j++){
			if (aMap[i][j] != getID()){ //排除自己
				if (aMap[i][j] > 0){
					if (manDis(argX, argY, i, j) <= 5)
						if (dPotentialValue > 0)
                            dPotentialValue *= (sqrt(0.0+getHP()/getMaxHP()) * 0.99);//如果距离过近，则危险度为1
						else
							dPotentialValue -= 1/100;
				}
			}
		}

	return dPotentialValue;
}
double st19::analyseAttack(int argX, int argY){//TODO:调整攻击策略
	double dTempValue = 0;
	if (getLevel() <= iEvolveLevel2)
	{
		if (aMap[argX][argY] == FOOD) {//攻击水草获得的价值
			dTempValue += (1 + m_min(getMaxHP(), getHP()+m_max(2,getMaxHP()/10)) - getHP());
			//水草的价值为1（经验) + 可以恢复的体力值
		}

		if (aMap[argX][argY] > 0) {//攻击鱼获得的价值
			int iKillHp = askHP(aMap[argX][argY]);
			if (getAtt() >= iKillHp)
				dTempValue += m_max(1, getLevel() / 2) + iKillHp/getAtt();
				//杀人的价值为经验，同等条件下杀血量最多的
		}
	}
	else
	{
        /*if (aMap[argX][argY] == FOOD) {//攻击水草获得的价值
			dTempValue += 1;//只考虑经验
        }*/

		if (aMap[argX][argY] > 0) {//攻击鱼获得的价值
			int iKillHp = askHP(aMap[argX][argY]);
			if (getAtt() >= iKillHp)
				dTempValue += iKillHp + 1/*保证杀人而不是吃水藻*/;
            else
                dTempValue += iKillHp / 100;
                //dTempValue += aSurviveTimes[aMap[argX][argY]] / 500;
		}
	}
	return dTempValue;
}

void st19::play()
{
	//初始化
	iTurn ++;//增加回合数
	refreshMap();//更新地图
    for (int i = 1; i <= N; i++)
        for (int j = 1; j <= M; j++)
            aSurviveTimes[aMap[i][j]]++;
	//决策
	iTargetX = getX(), iTargetY = getY();//移动目标
	dMoveValue = -dMax;//移动产生的价值,初始为0，然后为获得的经验+获得的生命值
	iAttackX = 0, iAttackY = 0;
	for (int i = 1; i <= N; i++)
		for (int j = 1; j <= M; j++)
			if ((manDis(getX(), getY(), i, j) <= getSp()) && (aMap[i][j] == EMPTY)){
				double dPositionValue = analysePosition(i, j);//在吃水草或者杀人的价值同等时由位置函数来决定
				refreshMove(i, j, 0, 0, dPositionValue);
				for (int iDir = 1; iDir <= 4; iDir++){
					refreshMove(i, j, i + aDirection[iDir][1], j + aDirection[iDir][2], dPositionValue + analyseAttack(i + aDirection[iDir][1], j + aDirection[iDir][2]));//更新
				}
			}


	//行动
	move(iTargetX, iTargetY);
	if (iAttackX != 0) attack(iAttackX, iAttackY);


	//分配点数（即时加点）
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
	//分配初始点数，开头为参数表
	const int iHealth = 4;
	const int iSpeed = 5;
	const int iStrength = 1;
	for (int i = 1; i <= iHealth; i++) increaseHealth();
	for (int i = 1; i <= iSpeed; i++) increaseSpeed();
	for (int i = 1; i <= iStrength; i++) increaseStrength();


	//初始化各变量
	iTurn = 0;
	for (int i = 0; i <= N+1; i++)//设置边界为0
		for (int j = 0; j <= M+1; j++)
			aMap[i][j] = 0;
    for (int i = 0; i < 100; i++)
        aSurviveTimes[i] = 0;

}

/** @brief (one liner)
  *
  * 更新地图
  */
void st19::refreshMap()
//更新地图
{
	for (int i = 1; i <= N; i++)
		for (int j = 1; j <= M; j++)
			aMap[i][j] = askWhat(i,j);

}

