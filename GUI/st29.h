#ifndef Veggie
#define Veggie
#include "fish.h"

#define veggie st29

class veggie: public fish {
private:
    static const int S_Range;
    static const int Speed_aim;
    static const int Fre_Str;
    static const int k;
    static const int My_N;
    static const int My_M;
    static const int d[4][2];
    int map[N + 5][M + 5], att_hp[N + 5][M + 5], att_s[N + 5][M + 5], de_s[N + 5][M + 5];
    int ID, posx, posy, Strength, Speed, Exp, MaxHP, Point, hp, Lev, Avg, Last_Str;
    int abs(int);
    void addPoints();
    void getCondition();
    bool canAttack(int, int);
    int vic_hp(int, int);
    int Defense(int, int);
    void myattack(int, int);
public:
    void play();
    void init();
    void revive(int& x, int&y);
};
#endif
