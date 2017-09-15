#ifndef AI_H
#define AI_H
#include "fish.h"
#include <cstdio>


class AI:virtual public fish{

    public:
    void init();

    void play();

    void revive(int&,int&);

};
#endif // AI_H
