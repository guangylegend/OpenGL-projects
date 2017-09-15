/*
	edly

	Just have fun
*/

#include <cstdio>
#include <vector>
#include <algorithm>
#include <ctime>

#include "st12.h"

const int initial_dps=1;
const int initial_speed=6;
const int initial_hp=3;
const int dead_time_limit=10;

void st12::add_dps() {increaseStrength();}
void st12::add_hp() {increaseHealth();}
void st12::add_speed() {increaseSpeed();}
int st12::dis(int i,int j,int p,int q) {return abs(i-p)+abs(j-q);}
int st12::random(int mo) {return ((rand()<<15)+rand())%mo;}

void st12::add_point(double dps,double hp,double speed)
{
	int hehe=1000000000;
	double p=1.*random(hehe)/hehe;
	if (p<dps+1e-6) {add_dps(); return;}
	if (p<dps+hp+1e-6) {add_hp(); return;}
	add_speed();
}

void st12::init()
{
	srand(time(0)+rand());
	go0[0]=0; go0[1]=0; go0[2]=1; go0[3]=-1;
	go1[0]=1; go1[0]=-1; go1[2]=0; go1[3]=0;
	go2[0]=0; go2[1]=1; go2[2]=2; go2[3]=3;
	add_point_time=0;
	dead_time=0;
	for (int i=0;i<initial_dps;i++) add_dps();
	for (int i=0;i<initial_hp;i++) add_hp();
	for (int i=0;i<initial_speed;i++) add_speed();
}

void st12::work(int i)
{
	move(attack_able_vec[i].x,attack_able_vec[i].y);
	attack(attack_able_vec[i].xx,attack_able_vec[i].yy);
}

void st12::funny_add_point()
{
	while (getPoint())
	{
		if (dead_time<dead_time_limit)
		{
			if (add_point_time<3)
			{
				add_point_time++;
		//		if (add_point_time%2==1)
				{
					add_hp();
					add_hp();
					add_speed();
				}
		//		else
		//		{
		//			add_hp();
		//			add_speed();
		//			add_speed();
		//		}
			}
			else if (getSp()>85) add_point(0.5,0.5,0.0);
			else add_point(0.25,0.45,0.3);
		}
		else
		{
			if (getAtt()<30)
			{
				add_dps();
				add_speed();
				add_speed();
			}
			else
			{
				add_speed();
				add_speed();
				add_speed();
			}
		}
	}
}

void st12::play()
{
	srand(time(0)+rand());
	funny_add_point();
	int max_hp=getMaxHP();
	int my_hp=getHP();
	int x=getX();
	int y=getY();
	for (int i=1;i<=40;i++)
		for (int j=1;j<=40;j++)
		{
			map[i][j]=askWhat(i,j);
			map_hp[i][j]=(map[i][j]>0)?askHP(map[i][j]):0;
			if (dis(i,j,x,y)<=getSp()) v[i][j]=1;
			else v[i][j]=0;
		}
	attack_able_vec.clear();
	for (int i=1;i<=40;i++)
		for (int j=1;j<=40;j++)
			if (map[i][j]!=EMPTY && dis(i,j,x,y)<=getSp()+1)
			{
				if (map[i][j]==getID()) continue;
				int ok=0;
				std::random_shuffle(go2,go2+4);
				for (int hehe=0;hehe<4;hehe++)
				{
					int k=go2[hehe];
					int ii=i+go0[k];
					int jj=j+go1[k];
					if (map[ii][jj]==EMPTY && v[ii][jj] && !ok)
					{
						ok=1;
						attack_able tmp(ii,jj,i,j,map[i][j],map_hp[i][j]);
						attack_able_vec.push_back(tmp);
					}
				}
			}
	std::sort(attack_able_vec.begin(),attack_able_vec.end());
	if (attack_able_vec.size()!=0)
	{
		double hpp=1.*getHP()/getMaxHP();
		if (dead_time<dead_time_limit && hpp<0.9 && attack_able_vec.back().map==FOOD)
		{
			int i=0;
			while (attack_able_vec[i].map!=FOOD) i++;
			work(i+random(attack_able_vec.size()-i));
		}
		else if (dead_time<dead_time_limit && random(20)==0) work(0);
		else if (getAtt()>=attack_able_vec.back().map_hp)
		{
			int i=0;
			while (attack_able_vec[i].map_hp>getAtt()) i++;
			work(i);
		}
		else work(0);
		funny_add_point();
	}
	else
		while (!move(random(40)+1,random(40)+1)) {}
}

void st12::revive(int &x,int &y)
{
	dead_time++;
	srand(time(0)+rand());
	x=random(40)+1;
	y=random(40)+1;
}
