#ifndef MAPBLOCK_H
#define MAPBLOCK_H
#include <QString>
#include "constant.h"
#include <vector>

class mapblock
{
public:
    QString add;
    int hp;
    int id;
    int lv;
    int maxhp;
    int sp;
    int att;
    int exp;
    int pstr;
    int psp;
    int phea;
    int plast;
    int x;
    int y;
    int score;
    bool hasmoved;
    bool hasattacked;
    int dead;
    int deadcount;
    QString name;
};

class playeryg:public mapblock
{
public:
    playeryg(int,int,int,QString);
    ~playeryg();
};

class sea:public mapblock
{
public:
    sea();
    ~sea();
};

class algae:public mapblock
{
public:
    algae();
    ~algae();
};

#endif // MAPBLOCK_H
