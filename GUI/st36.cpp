#include "st36.h"
#include "fish.h"
#include <cstdio>
#include <ctime>
#include <cstdlib> 

void st36::init(){
    increaseStrength();
	while (getPoint()){
		if (getPoint()) increaseHealth();
		if (getPoint()) increaseSpeed();
		if (getPoint()) increaseSpeed();
	}
}

void st36::play(){
    srand (time(NULL));
	while (getPoint()){
		if (getPoint())                increaseHealth();
        if (getPoint())                increaseSpeed();
        if (getPoint()>0 && getSp()>7) increaseStrength();
	}

	const int inf = (~0U>>1);
    int x, y, tmp, d, ds, maxd = 0, maxds = 0, canget = 0;

	for (int i = 1; i <= N; ++i)
        for (int j = 1; j <= M; ++j)
			if (askWhat(i,j) == EMPTY){

                int goal = abs(i-getX()) + abs(j-getY());
				if (goal > getSp()) continue;

				d = inf;
                for (int k = 1; k <= N; ++k)
                    for (int l = 1; l <= M; ++l)
                        if (askWhat(k,l) > 0){
                           tmp = abs(i-k) + abs(j-l);
                           if (tmp < d) d = tmp;
                        }                                             //最近的鱼

			    for (int d1 = -1; d1 < 2; ++d1)               //如果有可攻击对象
                    for (int d2=-1; d2 < 2; ++d2){
                        if (d1 == d2) continue;
                        if (d1+d2 == 0) continue;
                        if (x+d1>N && x+d1<1 && y+d2>M && y+d2<1) continue;
                        if (askWhat(x+d1,y+d2) != EMPTY){
                            if (askWhat(x+d1,y+d2) > 0){
                                if ( (askHP(askWhat(x+d1,y+d2)) ) > getAtt()) continue;
                            }
                            canget++;
                            ds = d;
                        }
                    }                                       //选择较为安全的位置
                if (canget > 0 && ds >= maxds){
                    if (ds > maxds)  maxds = ds, x = i, y = j;
                    if (ds == maxds && (rand()%2)) x = i, y = j;
                    }
                if (canget == 0 && d > maxd) maxd = d, x = i, y = j;
            }

	move(x,y);

    for (int d1 = -1; d1 < 2; ++d1)               //攻击
             for (int d2=-1; d2 < 2; ++d2){
                        if (d1 == d2) continue;
                        if (d1+d2 == 0) continue;
                        if (x+d1>N || x+d1<1 || y+d2>M || y+d2<1) continue;
                        if (askWhat(x+d1,y+d2) > 0){
                           if ( (askHP(askWhat(x+d1,y+d2)) ) > getAtt()) continue;
                        }
				        attack(x+d1,y+d2);
				        return;
            }
    }


void st36::revive(int& x,int& y){x = M; y = N;}
