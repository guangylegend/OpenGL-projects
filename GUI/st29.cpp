#include "fish.h"
#include "st29.h"
#include <cstring>
#include <cstdlib>
#define veggie st29

#define max(x, y) ((x > y) ? (x):(y))
const int veggie::S_Range = 5;
const int veggie::Speed_aim = max(N / 4, M / 4);
const int veggie::Fre_Str = 5;
const int veggie::k = 7;
const int veggie::My_N = 10;
const int veggie::My_M = 10;
const int veggie::d[4][2] = {1, 0, 0, 1, -1, 0, 0, -1};

int veggie::abs(int x) { return (x>0)?x:-x;}
void veggie::addPoints(){
     bool flag = 0;
     while (Point){
    	if (hp < Avg){
		increaseHealth();
    		MaxHP = getMaxHP();
	        hp = askHP(ID);
		Point = getPoint();
		continue;
	}
	if (Speed < Speed_aim && Last_Str <= Fre_Str){
		increaseSpeed();
		Speed = getSp();
		Point = getPoint();
		continue;
	}
	flag = 1;
	increaseStrength();
	Strength = getAtt();
	Point = getPoint();
    }
    if (flag) Last_Str = 0; else Last_Str ++;
}

void veggie::getCondition() {
    posx = getX();
    posy = getY();
    Lev = getLevel();
    Exp = getExp();
    Point = getPoint();
    ID = getID();
    hp = askHP(ID);
    Strength = getAtt();
    Speed = getSp();
    MaxHP = getMaxHP();
    Avg = 0;
    int Num = 0;
    memset(att_hp, -1, sizeof(att_hp));
    for (int  i = -Speed * S_Range; i <= Speed * S_Range; i ++){
    	for (int j = -Speed * S_Range; j <= Speed * S_Range; j ++){
		int x = posx + i, y = posy + j;
		if (x <= 0 || y <= 0 || x > N || y > M) continue;
		map[x][y] = askWhat(x, y);
		if (map[x][y] > 0){
			if (abs(i) + abs(j) <= Speed + 1)
				att_hp[x][y] = askHP(map[x][y]);
			Avg += att_hp[x][y]; Num ++;
		}
	}
    }
    Avg /= Num;
  }

bool veggie::canAttack(int x, int y) {
  	for (int i = 0; i < 4; i ++){
		int xx = x + d[i][0], yy = y + d[i][1];
		if (xx < 1 || yy < 1 || xx >= N || yy >= M) continue;
		if (map[xx][yy] > 0 && map[xx][yy] != ID && att_hp[xx][yy] <= Strength) return 1;
	}
	return 0;
  }

int veggie::vic_hp(int x, int y){
	int Max = 0;
  	 for (int i = 0; i < 4; i ++){
		int xx = x + d[i][0], yy = y + d[i][1];
		if (xx < 1 || yy < 1 || xx >= N || yy >= M) continue;
		if (map[xx][yy] > 0 && map[xx][yy] != ID && att_hp[xx][yy] <= Strength)
			Max= max(Max, att_hp[xx][yy]);
	}
	return Max;
  }

int veggie::Defense(int x, int y){
     int S = MAX_PLAYER;
     for (int  i = -Speed * S_Range; i <= Speed * S_Range; i ++){
    	for (int j = -Speed * S_Range; j <= Speed * S_Range; j ++){
		int x = posx + i, y = posy + j;
		if (x <= 0 || y <= 0 || x > N || y > M) continue;
		map[x][y] = askWhat(x, y);
		if (map[x][y] > 0) S --;
	}
    }
    return S;
  }

void veggie::myattack(int x, int y){
	int Max = 0, aim_X, aim_Y;
  	 for (int i = 0; i < 4; i ++){
		int xx = x + d[i][0], yy = y + d[i][1];
		if (xx < 1 || yy < 1 || xx >= N || yy >= M) continue;
		if (map[xx][yy] > 0 && map[xx][yy] != ID && att_hp[xx][yy] <= Strength)
		{	Max= max(Max, att_hp[xx][yy]); aim_X = xx; aim_Y = yy;}
	}
  	attack(aim_X, aim_Y);
  }

void veggie::init(){
	getCondition();
	Avg = max(6, Avg);
	addPoints();
}
void veggie::play() {
    //get condition
    getCondition();
    //add points
    addPoints();
    //find targets, move and attack
    memset(att_s, -1, sizeof(att_s));
    memset(de_s, -1, sizeof(de_s));
    for (int i = -Speed; i <= Speed; i ++)
	    for (int j = -Speed; j <= Speed; j ++){
	        int x= posx + i, y = posy + j;
	        if (x <= 0 || y <= 0 || x > My_N || y > My_M || map[x][y] != EMPTY)
		   continue;
		if (canAttack(x, y)) att_s[x][y] = vic_hp(x, y); else att_s[x][y] = 0;
	       	de_s[x][y] = Defense(x, y);
	    }
    int Max = -1, x = posx, y = posy;
    for (int i = 1; i <= N; i ++)
	    for (int j = 1; j <= M; j ++){
	    	if (att_s[i][j] != -1 && att_s[i][j] + k * de_s[i][j] > Max){
		   Max = att_s[i][j] + k * de_s[i][j];
		   x = i; y = j;
		}
	    }
    move(x, y);
    posx = getX();
    posy = getY();
    if (canAttack(posx, posy)) myattack(posx, posy);
};
void veggie::revive(int &x, int &y){
	x = rand() % My_N + 1; y = rand() % My_M + 1;
	return ;
}

