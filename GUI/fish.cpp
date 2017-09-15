#include "fish.h"

extern sys* syst;

fish::fish()
{
    ID=0;
    name = "";
}
void fish::setID(int i)
{
    this->ID = i;
}

bool fish::move(int x,int y)
{
    return syst->move(this->ID,x,y);
}

bool fish::attack(int x,int y)
{
    return syst->attack(this->ID,x,y);
}

int fish::getPoint()
{
    return syst->getPoint(this->ID);
}

int fish::getLevel()
{
    return syst->getLevel(this->ID);
}

int fish::getExp()
{
    return syst->getExp(this->ID);
}

int fish::getX()
{
    return syst->getX(this->ID);
}

int fish::getY()
{
    return syst->getY(this->ID);
}

int fish::getHP()
{
    return syst->getHP(this->ID);
}

int fish::getMaxHP()
{
    return syst->getHP(this->ID);
}

int fish::getAtt()
{
    return syst->getAtt(this->ID);
}

int fish::getSp()
{
    return syst->getSp(this->ID);
}

int fish::getID()
{
    return this->ID;
}

int fish::askWhat(int x,int y)
{
    return syst->askWhat(x,y);
}

int fish::askHP(int x)
{
    return syst->askHP(x);
}
int fish::getTotalPlayer()
{
    return syst->getTotalPlayer();
}

bool fish::increaseHealth()
{
    return syst->increaseHealth(this->ID);
}

bool fish::increaseStrength()
{
    return syst->increaseStrength(this->ID);
}

bool fish::increaseSpeed()
{
    return syst->increaseSpeed(this->ID);
}
