#include "st14.h"
#include <map>
#include <cmath>
#include <cstdio>
#include <vector>
#include <string>
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <time.h>
#define ran(n) (((rand() << 15) + rand()) % n)
#define kolim 10
#define kolel 10
#define htl 2.4
#define lpr 0.5
#define rev 0.5

st14::st14()
{
    go[0][0] = 0; go[0][1] = 1; go[1][0] = 0; go[1][1] = -1;
    go[2][0] = 1; go[2][1] = 0; go[3][0] = -1; go[3][1] = 0;
    for (int i = 0; i <= N + 1; i++)
        for (int j = 0; j <= M + 1; j++)
            map[i][j] = EMPTY;
}

void st14::init()
{
    for (int i = 0; i < 3; i++)
        increaseHealth();
    for (int i = 0; i < 6; i++)
        increaseSpeed();
    for (int i = 0; i < 1; i++)
        increaseStrength();
    sp = getSp(); hp = getHP(); dps = getAtt(); mhp = getMaxHP();
    flag = 0; lel = 1; exp = 0; id = getID();
    for (int i = 0; i < 100005; i++) a[i].z = 0;
    a[id].z = 1; ko = 0; tot = 0; ci = 0;
}

void st14::addPoint()
{
    srand(time(0) + ran(100)); lel = getLevel();
    while (getPoint())
    {
        int t = std::min(dps / 10, 6);
        double b[3];
        if (lel <= 3)
            b[0] = 0.2, b[1] = 0, b[2] = 0.8;
        else if (ko <= kolim)
            b[1] = 0.4 - t * 2 / 30.0, b[0] = 0.4 + (0.4 - b[1]) * 0.25, b[2] = 0.2 + (0.4 - b[1]) * 0.75;
        else
            b[0] = 0, b[1] = 0.4 - t * 2 / 30.0, b[2] = 0.6 + t * 2 / 30.0;
        double k = ran(100000) / 100000.0;
        if (k < b[0]) increaseHealth();
        else if (k < b[0] + b[1]) increaseStrength();
        else increaseSpeed();
    }
    sp = getSp(); mhp = getMaxHP(); dps = getAtt(); hp = getHP();
}

void st14::revenge()
{
    if (hp == getHP()) return;
    hp = getHP(); int ge = 0, kx = 0, ky = 0, num;
    for (int i = 0; i < 4; i++)
    if (map[px + go[i][0]][py + go[i][1]] != EMPTY && map[px + go[i][0]][py + go[i][1]] != FOOD)
        ge++, kx = px + go[i][0], ky = py + go[i][1];
    if (ge != 1) return;
    num = map[kx][ky];
    if (fabs(lel - a[num].lel) > 10 || ran(10000) / 10000.0 < rev) return;
    attack(kx, ky); cho = num;
}

void st14::analysis()
{
    for (int i = 1; i <= N; i++)
        for (int j = 1; j <= M; j++)
        {
            map[i][j] = askWhat(i, j);
            if (map[i][j] != EMPTY && map[i][j] != FOOD)
                if (a[map[i][j]].z == 0)
                {
                    q[++tot] = map[i][j]; a[map[i][j]].z = 1;
                    rec t;
                    t.hp = askHP(map[i][j]);
                    if (ci == 1) t.lel = t.mlel = 1, t.mhp = t.hp;
                    else
                    {
                        t.mlel = std::max(ci * lpr, 1.0); t.mhp = t.hp * 10;
                        t.lel = std::min(std::max((t.mhp - 6) / htl, 1.0), t.mlel);
                    }
                    a[map[i][j]] = t;
                }
                else if (map[i][j] != id)
                {
                    int k = map[i][j]; a[k].mlel += lpr;
                    if (a[k].flag)
                    {
                        a[k].hp = askHP(k);
                        if (a[k].hp > a[k].mhp)
                        {
                            a[k].lel = std::min(a[k].lel + (a[k].hp - a[k].mhp) / htl, a[k].mlel);
                            a[k].mhp = a[k].hp;
                        }
                    }
                    else{
                        a[k].hp = askHP(k);
                        int t = a[k].hp * 10;
                        if (t > a[k].mhp)
                        {
                            double de = std::min(a[k].lel + (t - a[k].mhp) / htl, a[k].mlel) - a[k].lel;
                            a[k].lel += de;
                            a[k].mhp += (int) (de * htl);
                        }
                    }
                }
            a[map[i][j]].flag = 2;
        }
}

void st14::search(int &kx1, int &ky1, int &ax1, int &ay1, double &t1, int &kx2, int &ky2, int &ax2, int &ay2, int &t2)
{
    for (int i = 1; i <= N; i++)
        for (int j = 1; j <= M; j++)
        if (map[i][j] == EMPTY && abs(i - px) + abs(j - py) <= sp || map[i][j] == id)
            for (int k = 0; k < 4; k++)
            {
                int tx = i + go[k][0], ty = j + go[k][1], num = map[tx][ty];
                if (map[tx][ty] != EMPTY && map[tx][ty] != id)
                    if (map[tx][ty] == FOOD)
                    {
                        int k = ran(100) + 1;
                        if (k > t2)
                        {
                            t2 = k; kx2 = i; ky2 = j; ax2 = tx; ay2 = ty;
                        }
                    }
                    else if (a[num].lel > t1 && a[num].hp <= dps)
                    {
                        t1 = a[num].lel; kx1 = i; ky1 = j; ax1 = tx; ay1 = ty;
                    }
            }
}

void st14::play()
{
    px = getX(); py = getY(); ci++; cho = -1;
    if (!flag)
    {
        flag = 1; hp = getHP();
        for (int i = 0; i < tot; i++)
            a[q[i]].flag = 1;
    }
    analysis();
    for (int i = 0; i < tot; i++)
    if (a[q[i]].flag == 2) a[q[i]].flag = 1;
    else a[q[i]].flag = 0;
    //if (lel > 50) revenge();
    double t1 = 0;
    int kx1, ky1, ax1, ay1, kx2, ky2, ax2, ay2, t2 = -1;
    if (cho == -1)
    {
        search(kx1, ky1, ax1, ay1, t1, kx2, ky2, ax2, ay2, t2);
        if (t2 != -1 && (fabs(t1) < 1e-8 || lel > 20 && hp * 1.0 / mhp < 0.9 && ko < kolim))
        {
            move(kx2, ky2); attack(ax2, ay2);
            cho = 0;
        }
        else if (fabs(t1) > 1e-8)
        {
            cho = map[ax1][ay1];
            move(kx1, ky1); attack(ax1, ay1);
        }
    }
    if (cho > 0)
    {
        int p = getExp();
        if (p > exp)
        {
            a[cho].mlel = (p - exp) * 2;
            a[cho].lel = a[cho].mlel;
            a[cho].mhp = std::min(a[cho].mhp, (int)(a[cho].lel * htl) * 2);
        }
    }
    else if (cho == -1)
    {
        int kx, ky;
        for (int i = 1; i <= 100000; i++)
        {
            kx = ran(N) + 1; ky = ran(M) + 1;
            if (abs(px - kx) + abs(py - ky) <= sp && map[kx][ky] == EMPTY)
                break;
        }
        move(kx, ky);
    }
    exp = getExp();
    px = getX(); py = getY();
    addPoint();
}

void st14::revive(int& x,int& y)
{
    ci++;
    if (lel > kolel) ko++;
    flag = 0;
    for (int i = 1; i <= tot; i++)
        a[q[i]].mlel += 0.5;
    x = ran(N) + 1;
    y = ran(M) + 1;
}
