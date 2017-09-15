#include "st16.h"
#include<iostream>
#include <cstdio>
#include <cmath>
#include <stdlib.h>

bool st16::reachAble(int x,int y,int speed){
	return (x!=getX()&&y!=getY()&&x<=N&&x>0&&y<=M&&y>0&&abs(x-getX())+abs(y-getY())<=speed);
}

int st16::countEnemy(int x,int y,int speed){
	int enemy = 0;
	for (int a=1;a<=N;++a)
		for (int b=1;b<=M;++b)
			if(reachAble(a,b,speed+1)){
			if(askWhat(a,b)>EMPTY) ++enemy;
		}
	return enemy;
}

int st16::countKillAble(int x,int y,int att,struct point save[]){
	int speed = getSp();
	int ID;
	int find = 0;
	for (int a=1;a<=N;++a)
		for (int b=1;b<=M;++b)
			if(reachAble(a,b,speed+1)){
			if((ID=askWhat(a,b))>EMPTY){
				if (askHP(ID)<=att){
					save[find].x = a;
					save[find].y = b;
					++find;
				}
			}
		}
		return find;
}

int st16::countFood(int x,int y,int speed,struct point save[]){
	int find = 0;
	for (int a=0;a<=N;++a)
		for (int b=0;b<=M;++b){
			if(reachAble(a,b,speed+1)&&askWhat(a,b)==FOOD){
					save[find].x = a;
					save[find].y = b;
					++find;
			}
		}
		return find;
}

bool st16::killLowHpPlayer(int number,struct point save[]){
	int min = 1000,m,x=getX(),y=getY(),p=0;
	if (number == 0) return 0;
	else{
		for (int i=0;i<number;++i){
			for (int a=-1;a<2;a+=2){
				int b=0;
				if (reachAble(save[i].x+a,save[i].y+b,getSp())&&askWhat(save[i].x+a,save[i].y+b)==EMPTY){
					m = countEnemy(save[i].x+a,save[i].y+b,getSp());
					if (m<min){
						min = m;
						x = save[i].x+a;
						y = save[i].y+b;
						p = i;
					}
				}
			}
			for(int b=-1;b<2;b+=2){
				int a=0;
				if (reachAble(save[i].x+a,save[i].y+b,getSp())&&askWhat(save[i].x+a,save[i].y+b)==EMPTY){
					m = countEnemy(save[i].x+a,save[i].y+b,getSp());
					if (m<min){
						min = m;
						x = save[i].x+a;
						y = save[i].y+b;
						p = i;
					}
				}
			}
		}
		move(x,y);
		attack(save[p].x,save[p].y);
		return 1;
	}
}

bool st16::killLowHpPlayerH(int number,struct point save[]){
	int max = 0,min=0,m,x,y,i,j;
	if (number == 0) return 0;
	else{
		for (int t=0;t<number;++t){
			if (askHP(askWhat(save[t].x,save[t].y))>max){
				max = askHP(askWhat(save[t].x,save[t].y));
				x = save[t].x;
				y = save[t].y;
			}
		}
			for (int a=-1;a<2;a+=2){
				int b=0;
				if (reachAble(x+a,y+b,getSp())&&askWhat(x+a,y+b)==EMPTY){
					m = countEnemy(x+a,y+b,getSp());
					if (m>min){
						min = m;
						i = a;
						j = b;
					}
				}
			}
			for(int b=-1;b<2;b+=2){
				int a=0;
				if (reachAble(x+a,y+b,getSp())&&askWhat(x+a,y+b)==EMPTY){
					m = countEnemy(x+a,y+b,getSp());
					if (m>min){
						min = m;
						i = a;
						j = b;
					}
				}
			}
		move(x+i,y+j);
		attack(x,y);
		return 1;
	}
}

bool st16::killFood(int number,struct point save[]){
	int min = 1000,m,x,y,p;
	if (number == 0) return 0;
	else{
		for (int i=0;i<number;++i){
			for (int a=-1;a<2;a+=2){
				int b=0;
				if (reachAble(save[i].x+a,save[i].y+b,getSp())&&askWhat(save[i].x+a,save[i].y+b)==EMPTY){
					m = countEnemy(save[i].x+a,save[i].y+b,getSp());
					if (m<min){
						min = m;
						x = save[i].x+a;
						y = save[i].y+b;
						p = i;
					}
				}
			}
			for (int b=-1;b<2;b+=2){
				int a=0;
				if (reachAble(save[i].x+a,save[i].y+b,getSp())&&askWhat(save[i].x+a,save[i].y+b)==EMPTY){
					m = countEnemy(save[i].x+a,save[i].y+b,getSp());
					if (m<min){
						min = m;
						x = save[i].x+a;
						y = save[i].y+b;
						p = i;
					}
				}
			}
		}
		if(min != 1000){
			move(x,y);
			attack(save[p].x,save[p].y);
			return 1;
		}
		else return 0;
	}
}

bool st16::defend(int att){
	int x = getX();
	int y = getY();
	int ex=x,ey=y;
	if (reachAble(x+1,y,getSp())&&(askWhat(x+1,y)>EMPTY)){
		ex = x+1;
		ey = y;
	}
	else if (reachAble(x-1,y,getSp())&&(askWhat(x-1,y)>EMPTY)){
		ex = x-1;
		ey = y;
	}
	else if (reachAble(x,y+1,getSp())&&(askWhat(x,y+1)>EMPTY)){
		ex = x;
		ey = y+1;
	}
	else if (reachAble(x,y-1,getSp())&&(askWhat(x,y-1)>EMPTY)){
		ex = x;
		ey = y-1;
	}
	if (ex==x&&ey==y) return 0;
	else if (getHP()/att>=askHP(askWhat(ex,ey))/getAtt()){
		attack(ex,ey);
		return 1;
	}
	else return 0;
}

bool st16::move2food(){
	int speed = getSp();
	int m,x=getX(),y=getY(),max= 0;
	for (int k=1;k<=M*N;++k){
		int i=rand()%N+1;
		int j=rand()%M+1;
		if (reachAble(i,j,getSp())&&askWhat(i,j)==EMPTY){
			m = countFood(i,j,speed+1,killAble);
			if (m>max){
				max = m;
				x = i;
				y = j;
				}
			}
		}
	if(x!=getX()&&y!=getY()){
		move(x,y);
		return 1;
	}
	else
		return 0;
}

bool st16::move2safe(){
	int speed = getSp();
	int m,x=getX(),y=getY(),min=1000;
	for (int k=1;k<=M*N;++k){
		int i=rand()%N+1;
		int j=rand()%M+1;
		if (reachAble(i,j,getSp())&&askWhat(i,j)==EMPTY){
			m = countEnemy(i,j,speed+1);
			if (m<=min){
				min = m;
				x = i;
				y = j;
			}
		}
	}
	if(x!=getX()&&y!=getY()){
		move(x,y);
		return 1;
	}
	else
		return 0;
}

bool st16::move2kill(){
	int speed = getSp();
	int m,x=getX(),y=getY(),max=-1;
	for (int k=1;k<=M*N;++k){
		int i=rand()%N+1;
		int j=rand()%M+1;
		if (reachAble(i,j,getSp())&&askWhat(i,j)==EMPTY){
			m = countEnemy(i,j,speed+1);
			if (m>max){
				max = m;
				x = i;
				y = j;
			}
		}
	}
	if(x!=getX()&&y!=getY()){
		move(x,y);
		return 1;
	}
	else
		return 0;
}

bool st16::runFar(){
	bool state = 0;
	int speed = getSp();
	for (int a=speed;a>=0;--a){
		if (state == 1) break;
		for (int b=speed-a;b>=0;++b) if(a!=0&&b!=0){
			if(x+a<=N&&y+b<=M&&askWhat(x+a,y+b)==EMPTY) {
				move(x+a,y+b);
				state = 1;
				break;
			}
			else if(x-a>0&&y+b<=M&&askWhat(x-a,y+b)==EMPTY) {
				move(x-a,y+b);
				state = 1;
				break;
			}
			else if(x+a<=N&&y-b>0&&askWhat(x+a,y-b)==EMPTY) {
				move(x+a,y-b);
				state = 1;
				break;
			}
			else if(x-a>0&&y-b>0&&askWhat(x-a,y-b)==EMPTY) {
				move(x-a,y-b);
				state = 1;
				break;
			}
		}

	}
    return state;
}

void st16::init(){
	circle = 1;
	deadtime = 0;
	while(getSp()<5){
		increaseSpeed();
	}
	if(countEnemy(getX(),getY(),5)<1&&getSp()<6)increaseSpeed();
	increaseStrength();
	while(getPoint())increaseHealth();
	lastHP = getHP();
}

void st16::play(){
	x = getX();
	y = getY();
	int enemyAtt = lastHP - getHP();
	if(10*getAtt()<getMaxHP()||getAtt()<10){
		if (!killLowHpPlayerH(countKillAble(x,y,getAtt(),killAble),killAble)){
			if (!killFood(countFood(x,y,getSp(),killAble),killAble)){
				if (enemyAtt>0){
					if(!defend(enemyAtt)){
						if(!move2food()){
							if (!killLowHpPlayerH(countKillAble(x,y,5*getAtt(),killAble),killAble)){
								if(!move2safe()){
									runFar();
								}
							}
						}
					}
				}
				else if (!move2food()){
					if(!move2safe()){
						runFar();
					}
				}
			}
		}
	}
	else{
		if(6*getMaxHP()>10*getHP()&&getMaxHP()>8*getAtt()) killFood(countFood(x,y,getSp(),killAble),killAble);
		else if (!killLowHpPlayerH(countKillAble(x,y,getAtt(),killAble),killAble))
		if (!killLowHpPlayerH(countKillAble(x,y,2*getAtt(),killAble),killAble))
		if (!killFood(countFood(x,y,getSp(),killAble),killAble))
		if (!killLowHpPlayerH(countKillAble(x,y,5*getAtt(),killAble),killAble)){
			if (!killFood(countFood(x,y,getSp(),killAble),killAble))
			if (!killLowHpPlayerH(countKillAble(x,y,10*getAtt(),killAble),killAble)){
				if (enemyAtt>0){
					if(!defend(enemyAtt)){
						if(!move2safe()){
							runFar();
						}
					}
				}
				else
					if(!move2kill()){
						runFar();
					}

			}
		}



	}
	if (getLevel()<10){
		while(getPoint()&&getLevel()%2==0){
			if(getHP()<getMaxHP())increaseHealth();
			if(getPoint())increaseStrength();
			if(getPoint())increaseSpeed();
			if(getPoint())increaseSpeed();
		}
		while(getPoint()&&getLevel()%2==1){
			if(3*getHP()<2*getMaxHP())increaseHealth();
			if(getPoint())increaseHealth();
			if(getPoint())increaseSpeed();
			if(getPoint())increaseSpeed();
		}
	}
	else if(getPoint()){
		if(countKillAble(x,y,2*getAtt(),killAble)==0&&getSp()>2*getAtt()) increaseStrength();
		if(countFood(x,y,getSp(),killAble)<1&&getSp()<20)increaseSpeed();
		while(getPoint()){
			int state = 0;
			for (int i=1;i<=N;++i){
				if (state == 1) break;
				for (int j=1;j<=M;++j){
					if (askWhat(i,j)>EMPTY&&askWhat(i,j)!=getID()){
						if (!deadtime||(askHP(askWhat(i,j))>=getMaxHP()&&getMaxHP()<2*getHP())){
							increaseHealth();
							state = 1;
							break;
						}
					}
				}
			}
			for (int i=1;i<=N;++i){
				if (state == 1) break;
				for (int j=1;j<=M;++j){
					if (askWhat(i,j)>EMPTY&&askWhat(i,j)!=getID()){
						if (4*getHP()>getMaxHP()&&askHP(askWhat(i,j))>=getMaxHP()&&getMaxHP()<8*getHP()){
							increaseHealth();
							state = 1;
							break;
						}
					}
				}
			}
		if(getMaxHP()>2*getAtt()||(getPoint()&&2*getAtt()<getMaxHP()&&getSp()>2*getAtt()))increaseStrength();
		if(getPoint()&&5*getSp()<2*getMaxHP())increaseSpeed();
		if(getPoint()&&8*getHP()>getMaxHP())increaseHealth();
		increaseSpeed();
		if(getAtt()<20)increaseStrength();
		}
	}

	lastHP = getHP();
	++ circle;
}

void st16::revive(int& x,int& y){
	++deadtime;
	circle += 2;
	x=23;
	y=17;
}
