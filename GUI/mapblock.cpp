#include "mapblock.h"

sea::sea()
{
    add = "sea";
    hp = 0;
    id = 0;
    lv = -1;
    maxhp = 0;
    sp = 0;
    att = 0;
    exp = 0;
    pstr = 0;
    psp = 0;
    phea = 0;
    plast = START_POINT;
    x = 0;
    y = 0;
    score = 0;
    hasmoved = false;
    hasattacked = false;
    dead = 0;
    deadcount = 0;
    name = "empty";
}
algae::algae()
{
    add = "algae";
    hp = 1;
    id = -1;
    lv = 0;
    maxhp = 0;
    sp = 0;
    att = 0;
    exp = 0;
    pstr = 0;
    psp = 0;
    phea = 0;
    plast = START_POINT;
    x = 0;
    y = 0;
    score = 0;
    hasmoved = false;
    hasattacked = false;
    dead = 0;
    deadcount = 0;
    name = "food";
}
playeryg::playeryg(int aid,int ax,int ay,QString str)
{
    add = "fish";
    hp = 0;
    maxhp = 0;
    sp = 0;
    id = aid;
    att = 0;
    exp = 0;
    lv = 1;
    pstr = 0;
    psp = 0;
    phea = 0;
    plast = START_POINT;
    x = ax;
    y = ay;
    score = 0;
    hasmoved = false;
    hasattacked = false;
    dead = 0;
    deadcount = 0;
    name = str;
}
