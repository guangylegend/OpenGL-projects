#ifndef ST10_H
#define ST10_H

#include "fish.h"
#include "cstdlib"
#include <algorithm>
#include <vector>


class mapinfo
{
    public:
       int type;
       int id;
};

class player
{
    public:
       player();
       int health;
       int speed;
       int x;
       int y;
};


class st10:virtual public fish{

    public:
    st10();

    void init();

    void play();

    void revive(int&,int&);

    void killfood();
    void killplayer();
    void kengdie();

    void addpoint(double,double,double);
    int random(int);

    int turn;
    int ax[4];
    int ay[4];
    int tx,ty,sx,sy,ans;
    int dead,reborn;
    int x,y,att,sp,hp,lv;
    int maxplayer;
    mapinfo* note[N*M+1];
    int maprank[N*M+1];

    player pla[MAX_PLAYER+1];
};
#endif // ST10_H
