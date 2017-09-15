#include "fish.h"
#include "cstdlib"
#pragma once

struct rec
{
    int z, mhp, hp, flag; //flag 1上盘出现了，0没出现
    double lel, mlel;
};

class st14:virtual public fish
{
    private:
        static const int lim = 100005;
        int sp, hp, exp, dps, mhp, px, py, lel, id, map[N + 5][M + 5], go[4][2], flag, tot, ci, q[10000], cho, ko; //flag = 0上一盘死了
        rec a[100005]; // 每个id对应的位置
        void revenge();
        void addPoint();
        void analysis();
        void search(int &, int &, int &, int &, double &, int &, int &, int &, int &, int &);
    public:
        st14();
        void init();
        void play();
        void revive(int&,int&);
};

//可自行增加所需函数所需函数及变量，但需保证上面每个函数的完整与可调用

//不要使用全局变量！！
