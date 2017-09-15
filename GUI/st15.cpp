#include<cmath>
#include "st15.h"
#include <cstdio>
#include <algorithm>
using namespace std;
#define wuhaobo st15

st15::st15(){

}

void wuhaobo::init()
{
    round = 0;
    increaseStrength();
    increaseHealth();   increaseHealth();  increaseHealth();
    increaseSpeed();    increaseSpeed();	increaseSpeed();    increaseSpeed();    increaseSpeed();    increaseSpeed();
}

void wuhaobo::jiadian()
{
    if(getLevel()<10)
    {
        increaseSpeed();    increaseSpeed();
        increaseHealth();
    }
    if(9<getLevel()&&getLevel()<19)
    {
        if(getLevel()%2==0)
        {
            increaseSpeed();    increaseSpeed();
            increaseStrength();
        }
        else
        {
            increaseSpeed();   increaseStrength();  increaseHealth();
        }
    }
    if(getLevel()>18&&getLevel()<28)
    {
        increaseSpeed();    increaseSpeed();
        increaseHealth();
    }
    if(getLevel()>27&&getLevel()<39)
    {
        increaseSpeed();    increaseSpeed();
        increaseStrength();
    }
    if(getLevel()>38)
    {
        increaseHealth();   increaseStrength();
        increaseStrength();
    }
}

void wuhaobo::play()
{
    bool flagmove=0;
    if (before != 1)
        round++;
    before = 0;
    if(round<11)
    {
        for(int i=1;i<=N;++i)
            for(int j=M;j>=1;--j)
                {
                    if (askWhat(i,j) == FOOD )
                    {
                        int dst=abs(i+1-getX())+abs(j-getY());
                        if (dst<=getSp()&&!flagmove)
                        {
                            move(i+1,j);
                            attack(i,j);
                            flagmove=1;
                        }
                        dst=abs(i-1-getX())+abs(j-getY());
                        if (dst<=getSp()&&!flagmove)
                        {
                            move(i-1,j);
                            attack(i,j);
                            flagmove=1;
                        }
                        dst=abs(i-getX())+abs(j+1-getY());
                        if (dst<=getSp()&&!flagmove)
                        {
                            move(i,j+1);
                            attack(i,j);
                            flagmove=1;
                        }
                        dst=abs(i-getX())+abs(j-1-getY());
                        if (dst<=getSp()&&!flagmove)
                        {
                            move(i,j-1);
                            attack(i,j);
                            flagmove=1;
                        }
                    }
                    while(getPoint()>0)
                    jiadian();
                }

        for(int i=1;i<=N;++i)
            for(int j=M;j>=1;--j)
                {
                    if(askWhat(i,j)>0&&askHP(askWhat(i,j))<=getAtt())
                    {
                        int dst=abs(i+1-getX())+abs(j-getY());
                        if (dst<=getSp()&&!flagmove)
                        {
                            move(i+1,j);
                            attack(i,j);
                            flagmove=1;
                        }
                        dst=abs(i-1-getX())+abs(j-getY());
                        if (dst<=getSp()&&!flagmove)
                        {
                            move(i-1,j);
                            attack(i,j);
                            flagmove=1;
                        }
                        dst=abs(i-getX())+abs(j+1-getY());
                        if (dst<=getSp()&&!flagmove)
                        {
                            move(i,j+1);
                            attack(i,j);
                            flagmove=1;
                        }
                        dst=abs(i-getX())+abs(j-1-getY());
                        if (dst<=getSp()&&!flagmove)
                        {
                            move(i,j-1);
                            attack(i,j);
                            flagmove=1;
                        }
                    }
                    while(getPoint()>0)
                    jiadian();
                }
    }
    if(round>10)
    {
        for(int i=1;i<=N;++i)
            for(int j=M;j>=1;--j)
                {
                    if(askWhat(i,j)>0&&askHP(askWhat(i,j))<=getAtt())
                    {
                        int dst=abs(i+1-getX())+abs(j-getY());
                        if (dst<=getSp()&&!flagmove)
                        {
                            move(i+1,j);
                            attack(i,j);
                            flagmove=1;
                        }
                        dst=abs(i-1-getX())+abs(j-getY());
                        if (dst<=getSp()&&!flagmove)
                        {
                            move(i-1,j);
                            attack(i,j);
                            flagmove=1;
                        }
                        dst=abs(i-getX())+abs(j+1-getY());
                        if (dst<=getSp()&&!flagmove)
                        {
                            move(i,j+1);
                            attack(i,j);
                            flagmove=1;
                        }
                        dst=abs(i-getX())+abs(j-1-getY());
                        if (dst<=getSp()&&!flagmove)
                        {
                            move(i,j-1);
                            attack(i,j);
                            flagmove=1;
                        }
                    }
                    while(getPoint()>0)
                    jiadian();
                }
    for(int i=1;i<=N;++i)
        for(int j=M;j>=1;--j)
            {
                if (askWhat(i,j) == FOOD )
                {
                    int dst=abs(i+1-getX())+abs(j-getY());
                    if (dst<=getSp()&&!flagmove)
                    {
                        move(i+1,j);
                        attack(i,j);
                        flagmove=1;
                    }
                    dst=abs(i-1-getX())+abs(j-getY());
                    if (dst<=getSp()&&!flagmove)
                    {
                        move(i-1,j);
                        attack(i,j);
                        flagmove=1;
                    }
                    dst=abs(i-getX())+abs(j+1-getY());
                    if (dst<=getSp()&&!flagmove)
                    {
                        move(i,j+1);
                        attack(i,j);
                        flagmove=1;
                    }
                    dst=abs(i-getX())+abs(j-1-getY());
                    if (dst<=getSp()&&!flagmove)
                    {
                        move(i,j-1);
                        attack(i,j);
                        flagmove=1;
                    }
                }
                while(getPoint()>0)
                jiadian();
            }
    }

	for(int i=1;i<=N;++i)
		for(int j=M;j>=1;--j)
			{
                if(askWhat(i,j)==FOOD)
                {
                    int dst=abs(i-getX())+abs(j-getY());
                    if(dst<=2*getSp())
                    {
                        for(int xx=max( 1 , getX()-getSp() );xx<=min( N , getX()+getSp() );++xx)
                            for(int yy=min( M , getY()+getSp() ); yy>=max( 1 , getY()-getSp() ) ; --yy)
                            {
                                dst=abs(xx-i)+abs(yy-j);
                                if( askWhat(xx,yy) == 0 && dst<=getSp()&&!flagmove)
                                {
                                    move(xx,yy);
                                    flagmove=1;
                                }
                            }
                    }
                }
            }
    int jlx,jly,jl=100;
    for(int xx=max( 1 , getX()-getSp() );xx<=min( N , getX()+getSp() );++xx)
        for(int yy=min( M , getY()+getSp() ); yy>=max( 1 , getY()-getSp() ) ; --yy)
        {
            if(askWhat(xx,yy)==0)
           {
                int dst=100;
                for(int i=1;i<=N;++i)
                    for(int j=M;j>=1;--j)
                    {
                        if(askWhat(i,j)>0)
                        dst=min(dst,abs(xx-i)+abs(yy-j));
                    }
                if(dst>jl)
                {
                    jlx=xx;
                    jly=yy;
                    jl=dst;
                }
           }
        }
   if(jl>8&&round<8&&!flagmove)
   {
       move(jlx,jly);
       flagmove=1;
   }

    for(int i=1;i<=N;++i)
		for(int j=M;j>=1;--j)
            {
                if(askWhat(i,j)>0 )
                {
                    int dst=abs(i+1-getX())+abs(j-getY());
                    if (dst<=getSp()&&!flagmove)
                    {
                        move(i+1,j);
                        attack(i,j);
                        flagmove=1;
                    }
                    dst=abs(i-1-getX())+abs(j-getY());
                    if (dst<=getSp()&&!flagmove)
                    {
                        move(i-1,j);
                        attack(i,j);
                        flagmove=1;
                    }
                    dst=abs(i-getX())+abs(j+1-getY());
                    if (dst<=getSp()&&!flagmove)
                    {
                        move(i,j+1);
                        attack(i,j);
                        flagmove=1;
                    }
                    dst=abs(i-getX())+abs(j-1-getY());
                    if (dst<=getSp()&&!flagmove)
                    {
                        move(i,j-1);
                        attack(i,j);
                        flagmove=1;
                    }
                }
                while(getPoint()>0)
                jiadian();
            }
}

void wuhaobo::revive(int& x,int& y)
{
    round+=2;
    before = 1;
    x=-1;
}
