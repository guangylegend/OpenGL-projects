#include "sys.h"
#include "mapblock.h"
#include "mainwindow.h"


extern playeryg* dat[MAX_PLAYER+1];
extern mapblock* mapyg[N+1][M+1];
extern std::vector<playeryg*> deadlist;
extern QLabel* pltest;
extern MainWindow* pMW;
extern int lmx,lmy,mx,my;
extern int laattx,laatty;
extern int gametype;
extern int totalplayer;

sys::sys()
{

}

sys::~sys()
{

}

bool sys::move(int id,int ax,int ay)
{
    if(dat[id]->hasmoved==true||N<ax||ax<=0||M<ay||ay<=0||dat[id]->sp<abs(ax-dat[id]->x)+abs(ay-dat[id]->y))return false;
    else if(mapyg[ax][ay]->add!="sea")return false;
    else
    {
            mapyg[ax][ay] = dat[id];
            mapblock* p = new sea;
            mapyg[dat[id]->x][dat[id]->y] = p;
            lmx = dat[id]->x;
            lmy = dat[id]->y;
            dat[id]->x = ax;
            dat[id]->y = ay;
            mx = ax;
            my = ay;
            dat[id]->hasmoved = true;
            if(gametype == 0)
            pMW->textout("player "+QString::number(dat[id]->id)+"("+dat[id]->name+") has moved to ("+QString::number(dat[id]->x)+","+QString::number(dat[id]->y)+")");
            return true;
    }
}

bool sys::attack(int id,int ax,int ay)
{
    if(dat[id]->hasattacked==true||N<ax||ax<=0||M<ay||ay<=0||(abs(ax-dat[id]->x)+abs(ay-dat[id]->y))>1)return false;
    else if(mapyg[ax][ay]->add=="sea")return false;
        else
        {
            if(mapyg[ax][ay]->add=="algae")
            {
                if(dat[id]->att>0)
                {
                    mapblock* p = new sea;
                    mapyg[ax][ay] = p;
                    dat[id]->hp = std::min(dat[id]->hp+std::min(dat[id]->maxhp/10,2),dat[id]->maxhp);
                    dat[id]->exp++;
                    if(gametype == 0)
                    pMW->textout("player "+QString::number(dat[id]->id)+"("+dat[id]->name+")"+" has ate the food");
                    if(dat[id]->exp>=(dat[id]->lv+2)*(dat[id]->lv+1)/2-1)
                    {
                        dat[id]->lv++;
                        dat[id]->plast+=LEVEL_POINT;
                        if(gametype == 0)
                        pMW->textout("player "+QString::number(dat[id]->id)+"("+dat[id]->name+")"+" has raised to level"+QString::number(dat[id]->lv));
                    }
                }
            }
            else if(mapyg[ax][ay]->add=="fish")
            {

                mapyg[ax][ay]->hp-=dat[id]->att;
                if(gametype == 0)
                pMW->textout("player "+QString::number(dat[id]->id)+"("+dat[id]->name+") has attacked player "+ QString::number(mapyg[ax][ay]->id)+"("+ mapyg[ax][ay]->name+")");
                if(mapyg[ax][ay]->hp<=0)
                {
                    deadlist.push_back(dat[mapyg[ax][ay]->id]);
                    dat[mapyg[ax][ay]->id]->dead = DEADTURN;
                    dat[mapyg[ax][ay]->id]->deadcount++;
                    if(gametype == 0)
                    pMW->textout("player "+QString::number( mapyg[ax][ay]->id)+"("+ mapyg[ax][ay]->name+") has dead");
                    dat[id]->exp+=std::max(mapyg[ax][ay]->lv/2,1);
                    if(dat[id]->exp>=(dat[id]->lv+2)*(dat[id]->lv+1)/2-1)
                    {
                        dat[id]->lv++;
                        dat[id]->plast+=LEVEL_POINT;
                        if(gametype == 0)
                        pMW->textout("player "+QString::number(dat[id]->id)+"("+dat[id]->name+")"+" has raised to level"+QString::number(dat[id]->lv));
                    }
                    dat[id]->score+=2*std::max(mapyg[ax][ay]->lv-dat[id]->lv,0);
                    mapblock* p = new sea;
                    mapyg[ax][ay] = p;
                }
            }
            laattx = ax;
            laatty = ay;
            dat[id]->hasattacked = true;
            return true;
        }
}

int sys::getPoint(int id)
{
    return dat[id]->plast;
}

int sys::getLevel(int id)
{
    return dat[id]->lv;
}

int sys::getExp(int id)
{
    return dat[id]->exp;
}

int sys::getX(int id)
{
    return dat[id]->x;
}

int sys::getY(int id)
{
    return dat[id]->y;
}

int sys::getHP(int id)
{
    return dat[id]->hp;
}

int sys::getMaxHP(int id)
{
    return dat[id]->maxhp;
}

int sys::getAtt(int id)
{
    return dat[id]->att;
}

int sys::getSp(int id)
{
    return dat[id]->sp;
}

int sys::askWhat(int x,int y)
{
    if(N<x||x<=0||M<y||y<=0)return -2;
    if(mapyg[x][y]->add=="sea")return 0;
    else if(mapyg[x][y]->add=="algae")return -1;
    else return mapyg[x][y]->id;
}

int sys::askHP(int id)
{
    if(0<id&&id<=totalplayer)
    return dat[id]->hp;
    else return -2;
}
int sys::getTotalPlayer()
{
    return totalplayer;
}

bool sys::increaseHealth(int id)
{
    if(dat[id]->plast<=0)return false;
    else
    {
        dat[id]->plast--;
        dat[id]->phea++;
        dat[id]->hp+=2;
        dat[id]->maxhp+=2;
        return true;
    }
}

bool sys::increaseStrength(int id)
{
    if(dat[id]->plast<=0)return false;
    else
    {
        dat[id]->plast--;
        dat[id]->pstr++;
        dat[id]->att++;
        return true;
    }
}

bool sys::increaseSpeed(int id)
{
    if(dat[id]->plast<=0)return false;
    else
    {
        dat[id]->plast--;
        dat[id]->psp++;
        dat[id]->sp++;
        return true;
    }
}
