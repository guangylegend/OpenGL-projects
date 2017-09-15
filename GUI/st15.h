#include "fish.h"
#include "cstdlib"
#pragma once

class st15: public fish{
    private:
    int before, round;
    void jiadian();
    public:
    st15();
    void init();

    void play();

    void revive(int&,int&);

};
