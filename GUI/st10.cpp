#include "st10.h"
#include <cstdio>
#include <cmath>
#include <cstring>
#include <stdlib.h>
#include <time.h>
player::player()
{
    this->health=0;
    this->speed=0;
}

st10::st10()
{
    turn=0;
    ax[0]=0;
    ax[1]=0;
    ax[2]=1;
    ax[3]=-1;
    ay[0]=-1;
    ay[1]=1;
    ay[2]=0;
    ay[3]=0;
    tx=0;
    ty=0;
    sx=0;
    sy=0;
    ans=80;
    x=0;
    y=0;
    att=0;
    sp=0;
    hp=0;
    lv=0;
    dead=0;
    reborn=0;
    maxplayer=0;
}

void st10::killfood()
{
    for(int i=1;i<=M;++i)
        {
            for(int j=1;j<=N;++j)
            {
                if(note[j+(i-1)*M]->type==-1)
                {
                    for(int l=0;l<=3;++l)
                    {
                        if(((abs(i+ay[l]-y)+abs(j+ay[l]-x))<=sp)&&(1<=(i+ay[l]))&&(M>=(i+ay[l]))&&(1<=(j+ax[l]))&&(N>=(j+ax[l])))
                        {
                            if(note[j+ax[l]+(i+ay[l]-1)*M]->type==0)
                            {
                                if(maprank[j+ax[l]+(i+ay[l]-1)*M]<=ans)//caution!!!!!!!!!!!!!!!!!!!!!!!!!!
                                {
                                    tx=j;
                                    ty=i;
                                    sx=j+ax[l];
                                    sy=i+ay[l];
                                    ans=maprank[j+ax[l]+(i+ay[l]-1)*M];
                                }
                            }
                        }
                    }

                }
            }
        }
        if(ans<80)
        {
            move(sx,sy);
            attack(tx,ty);
        }
}

void st10::killplayer()
{
    if(dead<=-1)
    {

       for(int i=1;i<=M;++i)
        {
            for(int j=1;j<=N;++j)
            {
                if((note[j+(i-1)*M]->type==1)&&(pla[note[j+(i-1)*M]->id].health<=att))
                {
                    for(int l=0;l<=3;++l)
                    {
                        if(((abs(i+ay[l]-y)+abs(j+ay[l]-x))<=sp)&&(1<=(i+ay[l]))&&(M>=(i+ay[l]))&&(1<=(j+ax[l]))&&(N>=(j+ax[l])))
                        {
                            if(note[j+ax[l]+(i+ay[l]-1)*M]->type==0)
                            {
                                if(maprank[j+ax[l]+(i+ay[l]-1)*M]<=ans)//caution!!!!!!!!!!!!!!!!!!!!!!!!!!
                                {
                                    tx=j;
                                    ty=i;
                                    sx=j+ax[l];
                                    sy=i+ay[l];
                                    ans=maprank[j+ax[l]+(i+ay[l]-1)*M];
                                }
                            }
                        }
                    }

                }
            }
        }
        if(ans<80)
        {
            move(sx,sy);
            attack(tx,ty);
        }
    }
    else
    {
        int a=0;
        for(int i=1;i<=M;++i)
         {
             for(int j=1;j<=N;++j)
             {
                 if((note[j+(i-1)*M]->type==1)&&(pla[note[j+(i-1)*M]->id].health<=att))
                 {
                     for(int l=0;l<=3;++l)
                     {
                         if(((abs(i+ay[l]-y)+abs(j+ay[l]-x))<=sp)&&(1<=(i+ay[l]))&&(M>=(i+ay[l]))&&(1<=(j+ax[l]))&&(N>=(j+ax[l])))
                         {
                             if(note[j+ax[l]+(i+ay[l]-1)*M]->type==0)
                             {
                                 if(pla[note[j+(i-1)*M]->id].health>=a)//caution!!!!!!!!!!!!!!!!!!!!!!!!!!
                                 {
                                     tx=j;
                                     ty=i;
                                     sx=j+ax[l];
                                     sy=i+ay[l];
                                     a=pla[note[j+(i-1)*M]->id].health;
                                 }
                             }
                         }
                     }

                 }
             }
         }
         if(a>0)
         {
             move(sx,sy);
             attack(tx,ty);
         }
    }

}

void st10::kengdie()
{
    for(int i=1;i<=M;++i)
     {
         for(int j=1;j<=N;++j)
         {
             if((note[j+(i-1)*M]->type==1)&&(pla[note[j+(i-1)*M]->id].health<=att))
             {
                 for(int l=0;l<=3;++l)
                 {
                     if(((abs(i+ay[l]-y)+abs(j+ay[l]-x))<=sp)&&(1<=(i+ay[l]))&&(M>=(i+ay[l]))&&(1<=(j+ax[l]))&&(N>=(j+ax[l])))
                     {
                         if(note[j+ax[l]+(i+ay[l]-1)*M]->type==0)
                         {
                             if(maprank[j+ax[l]+(i+ay[l]-1)*M]<=ans)//caution!!!!!!!!!!!!!!!!!!!!!!!!!!
                             {
                                 tx=j;
                                 ty=i;
                                 sx=j+ax[l];
                                 sy=i+ay[l];
                                 ans=maprank[j+ax[l]+(i+ay[l]-1)*M];
                             }
                         }
                     }
                 }

             }
         }
     }
     if(ans<80)
     {
         move(sx,sy);
         attack(tx,ty);
     }
}

void st10::init()
{
    for(int i=0;i<1;i++)
    {
        increaseStrength();
    }
    for(int i=0;i<3;i++)
    {
        increaseHealth();
    }
    for(int i=0;i<6;i++)
    {
        increaseSpeed();
    }
    for(int i=1;i<=maxplayer;i++)
    {
        pla[i].health=0;
        pla[i].speed=0;
        pla[i].x=0;
        pla[i].y=0;
    }
    maxplayer=getTotalPlayer();
}
int st10::random(int a) {return ((rand()<<15)+rand())%a;}

void st10::addpoint(double att,double hp,double sp)
{
    int c=1000000000;
    double p=1.*random(c)/c;
    if (p<att+1e-6)
    {
        increaseStrength();
        return;
    }
    if (p<att+hp+1e-6)
    {
        increaseHealth();
        return;
    }
    increaseSpeed();
}

void st10::play()
{
    srand((unsigned) time(NULL)+rand());
    memset(maprank,0,sizeof(maprank));
    tx=0;
    ty=0;
    sx=0;
    sy=0;
    ans=80;
    turn+=1;
    x=this->getX();
    y=this->getY();
    att=this->getAtt();
    sp=this->getSp();
    hp=this->getMaxHP();
    lv=this->getLevel();




    for(int i=1;i<=M;++i)
    {
       for(int j=1;j<=N;++j)
        {
            mapinfo* p = new mapinfo;
            note[j+(i-1)*M]=p;
            if(askWhat(j,i)==FOOD)
            {
                p->type=-1;
            }
            else if(askWhat(j,i)==EMPTY)
            {
                p->type=0;
            }
            else
            {
                p->type=1;
                p->id=askWhat(j,i);
                if(pla[p->id].x!=0)
                {
                    pla[p->id].speed=std::max(pla[p->id].speed,abs(j-pla[p->id].x)+abs(i-pla[p->id].y));
                }
                pla[p->id].x=j;
                pla[p->id].y=i;
                pla[p->id].health=askHP(p->id);
            }
        }
    }
    for(int i=1;i<=M;++i)
    {
       for(int j=1;j<=N;++j)
        {
            for(int l=1;l<=maxplayer;l++)
            {
                if((abs(pla[l].x-j)+abs(pla[l].y-i))<=pla[l].speed)
                {
                    maprank[j+(i-1)*M]+=1;
                }
            }
        }
    }


    while(getPoint())
    {


            if(dead<=5)
            {
                if(lv<=3)
                {
                    addpoint(0,0.4,0.6);
                }
                else
                {
                    if(turn>=200)
                    {
                        addpoint(0.3,0.4,0.3);
                    }
                    else
                    {
                        addpoint(0.1,0.4,0.5);
                    }


                }

            }
            else
            {
                    if(att<30)
                    {
                        addpoint(0.3,0,0.7);
                    }
                    else if(hp<=120)
                    {
                        addpoint(0.3,0.1,0.6);
                    }
                    else
                    {
                        addpoint(0.1,0,0.9);
                    }

            }
        }


    if(dead<=5&&reborn==1)
    {
        killfood();
    }

    killplayer();
    killfood();
    kengdie();
    reborn = 0;

}



void st10::revive(int& x,int& y)
{
      int tx,ty,ans;
      ans=80;
      for(int i=1;i<=M;++i)
      {
            for(int j=1;j<=N;++j)
            {
                if(maprank[j+(i-1)*M]<=ans)
                {
                    tx=j;
                    ty=i;
                    ans=maprank[j+(i-1)*M];
                }
            }
      }
      x=tx;
      y=ty;
      dead++;
      reborn=1;
}

