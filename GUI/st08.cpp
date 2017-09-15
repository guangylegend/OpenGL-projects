#include "st08.h"
#include<time.h>

void st08::init(){
    myID=getID();
	int i;
	for (i=1;i<=4;i++) increaseHealth();
	for (i=1;i<=5;i++) increaseSpeed();
	while (getPoint()) increaseStrength();
	dir[0][0]=1;dir[0][1]=0;dir[1][0]=0;dir[1][1]=1;
	dir[2][0]=-1;dir[2][1]=0;dir[3][0]=0;dir[3][1]=-1;
	srand(time(NULL));
	deadtimes=0;Rounds=0;
	memset(maxHP,0,sizeof(maxHP));
	memset(appealtimes,0,sizeof(appealtimes));
	memset(appeal,0,sizeof(appeal));
	for (i=1;i<=MAX_PLAYER;i++) Exp[i]=-1;
}

void st08::increaseRandPoint(int x,int y,int z)
{
	int i=rand() % 100;
	if (i < x) increaseHealth();
	else
	if (i < x+y) increaseStrength();
	else
	if (i < x+y+z) increaseSpeed();
}

int st08::calc_MaxHP(int x)
{
    int a[41],del=MAX_PLAYER;
    memcpy(maxHP,a,sizeof(a));
    std::sort(a+1,a+MAX_PLAYER+1);
    while (x>0)
    {
        del--;x--;
    }
    return a[del];
}

int st08::calc_format()
{
    if (deadtimes>= Rounds/15) return 3;
    if (myHP>=calc_MaxHP(7) && myAttack>=calc_MaxHP(15)) return 2;
    if (myLevel*10 >= Rounds && myHP>=calc_MaxHP(10)) return 1;
    else return 0;
}

void st08::increasePoint()
{
    if (mySpeed<15)
    {
        increaseRandPoint(0,0,100);
        increaseRandPoint(20,0,80);
        increaseRandPoint(60,40,0);
    }
    while (getPoint())
    {
        int format=calc_format();
        if (format == 1)
        {
            if (Rounds > 80)
            {
                if (Rounds>myAttack*10) increaseRandPoint(0,100,0);
                else if (myMaxHP<=myAttack*3) increaseRandPoint(80,0,20);
                else if (mySpeed*5 < Rounds) increaseRandPoint(30,0,70);
                else increaseRandPoint(50,20,30);
            }
            else increaseRandPoint(40,30,30);
        }
        else if (format == 2)
        {
            if (mySpeed > 90) increaseRandPoint(30,50,10);
            else if (myMaxHP < myAttack*4) increaseRandPoint(100,0,0);
            else increaseRandPoint(30,40,30);
        }
        else if (format == 3)
        {
            if (mySpeed < 70) increaseRandPoint(0,30,70);
            else increaseRandPoint(25,65,10);
        }
        else
        {
            if (Rounds<=80) increaseRandPoint(40,30,30);
            else
            {
                if (Rounds>=myAttack*10) increaseRandPoint(0,100,0);
                else if (myMaxHP <= calc_MaxHP(12)) increaseRandPoint(100,0,0);
                else if (mySpeed >= 55) increaseRandPoint(35,50,15);
                else increaseRandPoint(30,40,30);
            }
        }
    }
}

void st08::Ready()
{
	for (int i=1;i<=N;i++)
        for (int j=1;j<=M;j++)
        {
            int del=askWhat(i,j);
            if (del == myID) del=0;
            map[i][j]=del;
            if (del>0)
            {
                if (appeal[del] == Rounds-1)
                {
                    int Sp=abs(last_location[del][0]-i)+abs(last_location[del][1]-j);
                    if (Sp>Player_Speed[del]) Player_Speed[del]=Sp;
                    if (askHP(del) > maxHP[del]) maxHP[del]=askHP(del);
                }
                else
                {
                    int delHP=askHP(del)*10;
                    if (delHP>=maxHP[del]) maxHP[del]=delHP;
                }
                appeal[del]=Rounds;
                appealtimes[del]++;
                last_location[del][0]=i;last_location[del][1]=j;
            }
        }
    found = 0;
}

int  st08::SecondStepFood(int x,int y)
{
    int ans=0;
    for (int i=1;i<=N;i++)
        for (int j=1;j<=M;j++)
        if (abs(i-x)+abs(j-y)<=mySpeed && map[i][j] == -1) ans++;
    return ans;
}

void st08::EatFood()
{
    myx=getX();myy=getY();
    maxx=myx,maxy=myy;
    int moreFood=0;
    for (int i=1;i<=N;i++)
        for (int j=1;j<=M;j++)
         if (abs(myx-i)+abs(myy-j) <= mySpeed && map[i][j] == 0)
         {
             for (int k=0;k<=3;k++)
             {
                 int delx=i+dir[k][0],dely=j+dir[k][1];
                if (delx<=0 || dely<=0 || delx>N || dely>M) continue;
                 if (map[delx][dely] == -1)
                 {
                     int del=SecondStepFood(i,j);
                     if (max_value == 0 || moreFood<del)
                     {
                         max_value=1;
                         moreFood=del;
                         maxx=i;
                         maxy=j;
                         found=1;
                     }
                 }
             }
         }
    if (found == 1)
    {
        move(maxx,maxy);
        for (int k=0;k<=3;k++)
             {
                 int delx=getX()+dir[k][0],dely=getY()+dir[k][1];
                 if (map[delx][dely] == -1)
                 {
                    attack(delx,dely);
                    break;
                 }
             }
    }
    max_value=0;
    myx=getX();myy=getY();
}


void st08::AttackFish()
{
    myx=getX();myy=getY();
    max_value=0;
    for (int i=1;i<=N;i++)
        for (int j=1;j<=M;j++)
        if (abs(myx-i)+abs(myy-j) <= mySpeed && map[i][j] == 0)
        {
            for (int k=0;k<=3;k++)
            {
                int delx=i+dir[k][0],dely=j+dir[k][1];
                if (delx<=0 || dely<=0 || delx>N || dely>M) continue;
                int del=map[delx][dely];
                if (del > 0 && askHP(del)<=myAttack)
                {
                    int value=Exp[del];
                    if (value == -1) value=Rounds/40;
                    if (value > max_value)
                    {
                        max_value=value;
                        maxx=i;maxy=j;
                        found=1;
                    }
                }
            }
        }
    if (found == 1)
    {
        move(maxx,maxy);
        max_value=0;
        for (int k=0;k<=3;k++)
        {
            int delx=getX()+dir[k][0],dely=getY()+dir[k][1];
            if (delx<=0 || dely<=0 || delx>N || dely>M) continue;
            int del=map[delx][dely];
            if (del>0 && askHP(del)<=myAttack)
            {
                int value=Exp[del];
                if (Exp[del] == -1) value=Rounds/40;
                if (value > max_value)
                {
                    max_value=value;
                    maxx=delx;maxy=dely;
                }
            }
        }
        attack(maxx,maxy);
    }
    max_value=0;
    myx=getX();myy=getY();
}

int st08::NearToMe(int x,int y)
{
    int ans=0;
    for (int i=1;i<=N;i++)
        for (int j=1;j<=M;j++)
        if (map[i][j]>0 && abs(x-i)+abs(y-j)<=mySpeed)
            ans++;
    return ans;
}

void st08::FindFood()
{
    int dist=100;
    for (int i=1;i<=N;i++)
        for (int j=1;j<=M;j++)
        {
            int sum;
            int value=(4-((abs(i-myx)+abs(j-myy))/mySpeed))*1000+SecondStepFood(i,j);
            if (value > max_value || (sum=NearToMe(i,j))<dist)
            {
                max_value=value;
                maxx=i;maxy=j;
                dist=sum;
                found=1;
            }
        }
    if (found == 1)
    {
        int dist = 1000,delx,dely;
        for (int i=1;i<=N;i++)
            for (int j=1;j<=M;j++)
            if (abs(i-myx)+abs(j-myy)<=mySpeed && map[i][j] == 0)
            {
                if (abs(i-maxx)+abs(j-maxy)<dist)
                {
                    dist=abs(i-maxx)+abs(j-maxy);
                    delx=i;dely=j;
                }
            }
        move(delx,dely);
        int max_HP = 0;
        for (int k=0;k<=3;k++)
        {
            delx=getX()+dir[k][0],dely=getY()+dir[k][1];
            if (delx<=0 || dely<=0 || delx>N || dely>M) continue;
            int del=map[delx][dely];
            if (del > 0 && askHP(del)>max_HP)
            {
                maxx=delx;maxy=dely;
                max_HP=askHP(del);
            }
        }
        if (max_HP !=0) attack(maxx,maxy);
    }
    max_value=0;
    myx=getX();myy=getY();
}

void st08::DecHP()
{
    int max_HP=0;
    for (int i=1;i<=N;i++)
        for (int j=1;j<=M;j++)
        if (abs(i-myx)+abs(j-myy)<=mySpeed && map[i][j] == 0)
        {
            for (int k=0;k<=3;k++)
            {
                int delx=i+dir[k][0],dely=j+dir[k][1];
                if (delx<=0 || dely<=0 || delx>N || dely>M) continue;
                int del=map[delx][dely];
                if (del>0)
                {
                    if (askHP(del) > max_HP && askHP(del)<=4*myAttack)
                    {
                        maxx=i;maxy=j;
                        max_HP=askHP(del);
                        found = 1;
                    }
                }
            }
        }
    if (found == 1)
    {
        move(maxx,maxy);
        for (int k=0;k<=3;k++)
        {
            int delx=getX()+dir[k][0],dely=getY()+dir[k][1];
            if (delx<=0 || dely<=0 || delx>N || dely>M) continue;
            int del=map[delx][dely];
            if (del>0)
            {
                if (askHP(del) == max_HP)
                {
                    maxx=delx;maxy=dely;
                }
            }
        }
        if (!(maxx==getX() && maxy==getY())) attack(maxx,maxy);
    }
    max_value=0;
    myx=getX();myy=getY();
}

void st08::Naughty()
{
    for (int i=1;i<=N;i++)
        for (int j=1;j<=M;j++)
        if (abs(myx-i)+abs(myy-j)<=mySpeed && map[i][j] == 0)
        {
            for (int k=0;k<=3;k++)
            {
                int delx=i+dir[k][0],dely=j+dir[k][1];
                if (delx<=0 || dely<=0 || delx>N || dely>M) continue;
                int del=map[delx][dely];
                if (del != 0)
                {
                    int value = rand() % 10000 + 1;
                    if (value > max_value)
                    {
                        max_value = value;
                        maxx=i;maxy=j;
                        found = 1;
                    }
                }
            }
        }
    if (found == 1)
    {
        move(maxx,maxy);
        for (int k=0;k<=3;k++)
        {
            int delx=getX()+dir[k][0],dely=getY()+dir[k][1];
            if (delx<=0 || dely<=0 || delx>N || dely>M) continue;
            int del=map[delx][dely];
            if (del != 0)
            {
                attack(delx,dely);
                myx=getX();myy=getY();
                max_value=0;
                return;
            }
        }
    }
}

void st08::play()
{
	myAttack=getAtt();mySpeed=getSp();
	myx=getX();myy=getY();
	myHP=getHP();myMaxHP=getMaxHP();
	myExp=getExp();myLevel=getLevel();Rounds++;
	Ready();
	if (myAttack <= 2)
	{
	    EatFood();
	    if (found == 0) AttackFish();
	    if (found == 0) FindFood();
	    if (found == 0) Naughty();
	}
	else if (Rounds <= 150)
	{
	    AttackFish();
        if (found == 0) EatFood();
        if (found == 0) DecHP();
        if (found == 0) Naughty();
	}
	else
	{
	    AttackFish();
	    if (found == 0)
	    if (myHP*0.8 >= myMaxHP)
	    {
            DecHP();
            if (found == 0) EatFood();
	    }
	    else
	    {
	        EatFood();
	        if (found == 0) DecHP();
	    }
	    if (found == 0) Naughty();
	}
    if (getPoint()) increasePoint();
}

void st08::revive(int& x,int& y)
{
	Rounds++;
	deadtimes++;
	max_value=-110000;maxx=0;maxy=0;
	int i,j,k;
	for (i=1;i<=N;i++)
		for (j=1;j<=M;j++)
		if (map[i][j] == EMPTY)
		{
			int delvalue=0;
			for (k=1;k<=MAX_PLAYER;k++)
			if (last_location[k][0]!=0 && k != myID)
				if (abs(i-last_location[k][0])+abs(j-last_location[k][1]) <= Player_Speed[k])
					delvalue-=20;
			int deli,delj;
			for (deli=1;deli<=N;deli++)
				for (delj=1;delj<=M;delj++)
				if (map[deli][delj] == FOOD && abs(i-deli)+abs(j-delj)<=mySpeed)
					delvalue+=1;
			if (max_value<delvalue)
			{
				max_value=delvalue;
				maxx=i;maxy=j;
			}
		}
	x=maxx;y=maxy;
}
