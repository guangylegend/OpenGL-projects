#pragma once
//#define DEBUG
#include "fish.h"

#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <cstring>
#include <vector>

/*#ifdef DEBUG
#include <fstream>
#include <iomanip>
#include <iostream>
#endif*/

using namespace std;

#define for1(i,a,b) for(i=a;i<=b;i++)
#define for2(i,a,b) for(i=a;i>=b;i--)

class st06:public fish{
private:
	class data{
		public:
		static const int QAttLimitSize=30;
		static double aveAtt;
		static vector< pair<int,int> > QAtt;
		static void aveAttStatistic(int v,int round);
		static double getAveAtt();
		
		int HP,MaxHP,Speed,Speednd,Att,Point,Level,Exp,X,Y;
		int THP,TMaxHP,TSpeed,TSpeednd,TAtt,TLevel,TX,TY;
		
		data();
		void wHP(int v,int round);
		void wMaxHP(int v,int round);
		void wAtt(int v,int round);
		void wSpeed(int v,int round);
		void wXY(int x,int y,int round);
		void wLevel(int v,int round);
		int rLevel(int round);
		int rSpeed(int round);
		double rAtt(int round);
	};
	static const int X[4],Y[4];
	static const double eps;
	double RATE,T;
	int round,tot;
	
	static bool legal(int x,int y);
	static bool Ran(const double pass_rate);
	static int Choose(int a,int b,int c);
	bool check(int x,int y);
	bool ifAccept(double a,double b);
	void refresh(bool flag=false);
	void addPoint();
	void paint(int x,int y,int r,double v);
	void access();
	void moving();
	void attacking();
	
public:
	data my;
	data other[MAX_PLAYER+3];
	int ID,death;
	int map[N+3][M+3],past[N+3][M+3];
	bool present[MAX_PLAYER+3],cur[MAX_PLAYER+3];
	double value[N+3][M+3];
	//temporary
	int q[N*M+3][2];
	bool f[N+3][M+3];
	
    void init();
    void play();
    void revive(int &x,int &y);
};


/*
	class data static
*/
double st06::data::aveAtt=0;
vector< pair<int,int> > st06::data::QAtt;

void st06::data::aveAttStatistic(int v,int round){
	if (v+v<getAveAtt())return;
	QAtt.push_back(pair<int,int>(round,v));
	aveAtt=0;
	if (QAtt.size()>QAttLimitSize){
		QAtt.erase(QAtt.begin());
	}
}

double st06::data::getAveAtt(){
	if (aveAtt<1){
		vector< pair<int,int> >::iterator i;
		aveAtt=1;
		double sum=0;
		for(i=QAtt.begin();i!=QAtt.end();i++){
			sum+=i->first;
		}
		for(i=QAtt.begin();i!=QAtt.end();i++){
			aveAtt+=(i->first)*(i->second)/sum;
		}
	}
	return aveAtt*((rand()%100)/100.0+1);
}

/*
	class data
*/
st06::data::data(){
	Speed=Speednd=1;
}

void st06::data::wHP(int v,int round){
	if (v>HP){
		wMaxHP(v,round);
	}
	else if (HP>v){
		wAtt(HP-v,round);
	}
	THP=round;
	HP=v;
}

void st06::data::wMaxHP(int v,int round){
	if (v>MaxHP){
		MaxHP=v;
		TMaxHP=round;
	}
}

void st06::data::wAtt(int v,int round){
	if (v>Att){
		Att=v;
		TAtt=round;
	}
	data::aveAttStatistic(v,round);
}

void st06::data::wSpeed(int v,int round){
	if (v>Speednd){
		Speednd=v;
		TSpeednd=round;
	}
	if (Speed<Speednd){
		swap(Speed,Speednd);
		TSpeed=round;
	}
}

void st06::data::wXY(int x,int y,int round){
	wSpeed(abs(x-X)+abs(y-Y),round);
	X=x;
	Y=y;
	TX=TY=round;
}

void st06::data::wLevel(int v,int round){
	Level=v;
	TLevel=round;
}

int st06::data::rLevel(int round){
	return Level;
}

int st06::data::rSpeed(int round){
	if ((Speednd+Speed)/2>60)return 80;
	else return (Speednd+Speed)/2;
}

double st06::data::rAtt(int round){
	return getAveAtt();
}
/*
	static varible
*/
const int st06::X[4]={0,0,1,-1};
const int st06::Y[4]={1,-1,0,0};
const double st06::eps=1e-8;
/*
	static function
*/
bool st06::legal(int x,int y){
	return (x>0 && y>0 && x<=N && y<=M);
}

bool st06::Ran(const double pass_rate){
	return (rand()%32767<pass_rate*32766+eps);
}

int st06::Choose(int a,int b,int c){
	int sum=a+b+c;
	int v=rand()%sum;
	if (v<a)return 0;
	if (v<a+b)return 1;
	return 2;
}
/*
	private function
*/
bool st06::check(int x,int y){
	int i;
	if (map[x][y]!=EMPTY)return false;
	for1(i,0,3){
		if (legal(x+X[i],y+Y[i]) && map[x+X[i]][y+Y[i]]!=EMPTY && map[x+X[i]][y+Y[i]]!=ID)return true;
	}
	return false;
}

bool st06::ifAccept(double a,double b){
	return (a<b || Ran(exp(b-a)/T));
}

void st06::refresh(bool flag){
	int i,j;
	memset(cur,0,sizeof(cur));
	memcpy(past,map,sizeof(map));
	for1(i,1,N){
		for1(j,1,M){
			map[i][j]=fish::askWhat(i,j);
			if (map[i][j]!=EMPTY && map[i][j]!=FOOD){
				cur[map[i][j]]=true;
			}
		}
	}
	if (my.HP>fish::getHP())data::aveAttStatistic(my.HP-fish::getHP(),round);
	my.HP=fish::getHP();
	my.MaxHP=fish::getMaxHP();
	my.Speed=fish::getSp();
	my.Att=fish::getAtt();
	my.Point=fish::getPoint();
	my.Level=fish::getLevel();
	my.Exp=fish::getExp();
	my.X=fish::getX();
	my.Y=fish::getY();
	
	if (!flag)return;
	for1(i,1,N){
		for1(j,1,M){
			int k=map[i][j];
			if (k!=EMPTY && k!=FOOD && k!=ID){
				present[k]=true;
				other[k].wHP(fish::askHP(k),round);
				other[k].wXY(i,j,round);
			}
			if (past[i][j]!=EMPTY && past[i][j]!=FOOD && past[i][j]!=ID && !cur[past[i][j]] && round!=1){
				other[past[i][j]].wHP(0,round);
			}
		}
	}
	int sum=0;
	for1(i,1,MAX_PLAYER)sum+=present[i];
	tot=max(tot,sum);
}

void st06::addPoint(){
	while (fish::getPoint()){
		int k=Choose(3,8,15);
		if (round>10)k=Choose(10,5,25);
		switch (k){
			case 0:{
				fish::increaseHealth();
				break;
			}
			case 1:{
				fish::increaseStrength();
				break;
			}
			case 2:{
				fish::increaseSpeed();
			}
		}
	}
}

void st06::paint(int x,int y,int R,double v){
	int i,h=0,r=1;
	int Tot[N*M+3];
	double tmp[N+3][M+3];
	memset(f,0,sizeof(f));
	memset(tmp,0,sizeof(tmp));
	f[x][y]=true;
	tmp[x][y]=v;
	value[x][y]+=v;
	q[h][0]=x;
	q[h][1]=y;
	Tot[h]=R;
	while (h<r){
		for1(i,0,3){
			if (Tot[h]==0)continue;
			x=q[h][0]+X[i];
			y=q[h][1]+Y[i];
			if (legal(x,y) && !f[x][y]){
				f[x][y]=true;
				tmp[x][y]=tmp[q[h][0]][q[h][1]]*RATE;
				value[x][y]+=tmp[x][y];
				q[r][0]=x;
				q[r][1]=y;
				Tot[r]=Tot[h]-1;
				r++;
			}
		}
		h++;
	}
}

void st06::access(){
	memset(value,0,sizeof(value));
	int i,j;
	for1(i,1,N){
		for1(j,1,M){
			if (map[i][j]==FOOD){
				paint(i,j,my.Speed,-double(my.MaxHP+10)/(my.HP+10));
			}
			if (map[i][j]!=EMPTY && map[i][j]!=FOOD && map[i][j]!=ID){
				int k=map[i][j];
				if (abs(my.X-i)+abs(my.Y-j)<=my.Speed+1){
					if (other[k].HP<=my.Att){
						paint(i,j,2,-max(1.0,other[k].rLevel(round)/2.0)*100);
					}else{
						paint(i,j,2,-(my.HP-int((other[k].HP-my.Att)/double(my.Att)+0.5)*other[k].rAtt(round)));
					}
				}
				if (abs(my.X-i)+abs(my.Y-j)<=other[k].rSpeed(round)+1){
					paint(i,j,other[k].rSpeed(round)+1,-(my.HP-other[k].rAtt(round)-int(other[k].HP/double(my.Att)+0.5)*other[k].rAtt(round)));
				}
				else{
					paint(i,j,other[k].rSpeed(round)+1,-(my.HP-int(other[k].HP/double(my.Att)+0.5)*other[k].rAtt(round)));
				}
			}
		}
	}
	T=0;
	for1(i,1,N){
		for1(j,1,M){
			T+=abs(value[i][j]);
		}
	}
	T/=100;
}

void st06::moving(){
	int i,h=0,r=1,x=my.X,y=my.Y,p=0;
	int Tot[N*M+3];
	memset(f,0,sizeof(f));
	f[x][y]=true;
	q[h][0]=x;
	q[h][1]=y;
	Tot[h]=my.Speed;
	while (h<r){
		for1(i,0,3){
			if (Tot[h]==0)continue;
			x=q[h][0]+X[i];
			y=q[h][1]+Y[i];
			if (legal(x,y) && !f[x][y]){
				f[x][y]=true;
				q[r][0]=x;
				q[r][1]=y;
				Tot[r]=Tot[h]-1;
				r++;
			}
		}
		h++;
	}
	for1(i,0,r-1){
		x=q[i][0];
		y=q[i][1];
		if (check(x,y) && (p==0 || ifAccept(value[x][y],value[q[p][0]][q[p][1]]))){
			p=i;
		}
	}
	if (p==0){
		for1(i,0,r-1){
			x=q[i][0];
			y=q[i][1];
			if (map[x][y]==EMPTY && (p==0 || ifAccept(value[x][y],value[q[p][0]][q[p][1]]))){
				p=i;
			}
		}
	}
	fish::move(q[p][0],q[p][1]);
}

void st06::attacking(){
	int i,x,y,x0=0,y0=0;
	for1(i,0,3){
		x=my.X+X[i];
		y=my.Y+Y[i];
		if (x>0 && y>0 && x<=N && y<=M && map[x][y]!=EMPTY && map[x][y]!=ID){
			if (x0==0 || ifAccept(value[x][y],value[x0][y0])){
				x0=x;
				y0=y;
			}
		}
	}
	x=x0;
	y=y0;
	if (x==0)return;
	int k=map[x][y];
	fish::attack(x,y);
	if (k!=FOOD && fish::getExp()>my.Exp){
		other[k].wLevel((fish::getExp()-my.Exp)*2,round);
	}
}
/*
	main function
*/
void st06::init(){
	memset(other,0,sizeof(other));
	memset(past,0,sizeof(past));
	memset(map,0,sizeof(map));
	memset(present,0,sizeof(present));
	ID=fish::getID();
	present[ID]=true;
	srand(time(NULL));
	RATE=0.7;
	round=0;
	tot=0;
	death=0;
	
	fish::increaseHealth();
	fish::increaseHealth();
	fish::increaseHealth();
	fish::increaseHealth();
	fish::increaseStrength();
	fish::increaseStrength();
	fish::increaseSpeed();
	fish::increaseSpeed();
	fish::increaseSpeed();
	fish::increaseSpeed();
	
	addPoint();
}

void st06::play(){
	round++;
	addPoint();
	refresh(true);
	access();
	moving();
	refresh();
	attacking();
	addPoint();
	//cout<<data::getAveAtt()<<endl;
}

void st06::revive(int &x,int &y){
	round++;
	death++;
	data::aveAttStatistic(my.HP,round);
	memset(value,0,sizeof(value));
	int i,j,x0=0,y0=0;
	x=0;
	for1(i,1,N){
		for1(j,1,M){
			if (map[i][j]!=EMPTY && map[i][j]!=FOOD){
				paint(i,j,other[map[i][j]].rSpeed(round),other[map[i][j]].HP-my.Att+other[map[i][j]].rSpeed(round));
			}
		}
	}
	for1(i,1,N){
		for1(j,1,M){
			if (x0==0 || ifAccept(value[i][j],value[x0][y0])){
				x0=i;
				y0=j;
			}
		}
	}
	x=x0;
	y=y0;
}

