#include "st34.h"

using namespace std;

void st34::init()
{
    d[0][0] = -1;
    d[0][1] = 0;
    d[1][0] = 1;
    d[1][1] = 0;
    d[2][0] = 0;
    d[2][1] = -1;
    d[3][0] = 0;
    d[3][1] = 1;
    for (int i = 1; i <= 4; ++i)
        increaseHealth();
    for (int i = 1; i <= 1; ++i)
        increaseStrength();
    for (int i = 1; i <= 5; ++i)
        increaseSpeed();
    sx = -1;
    sy = -1;
}

void st34::play()
{
    sx = -1;
    sy = -1;
    while (getPoint())
        increasePoint();
    int x0 = 0, y0 = 0, a = -1;
    if (ePoint(getX(), getY(), x0, y0, a) >= 0)
        if ((x0 >= 1) && (x0 <= N) && (y0 >= 1) && (y0 <= N))
        {
            move(x0, y0);
            if ((a >= 0) && (a <= 3))
                attack(x0 + d[a][0], y0 + d[a][1]);
            return;
        }
    int x1 = -1, y1 = -1, x2 = -1, y2 = -1, minsv = 999, msv = 999, maxv = -1;
    for (int x = 1; x <= N; ++x)
        for (int y = 1; y <= M; ++y)
        {
            int sv = safe(x, y);
            if (sv < minsv)
            {
                minsv = sv;
                sx = x;
                sy = y;
            }
            if (dis(x, y) <= getSp())
            {
                int value = ePoint(x, y, x0, y0, a);
                if (value > maxv)
                {
                    maxv = value;
                    x1 = x;
                    y1 = y;
                }
                if (sv < msv)
                {
                    msv = sv;
                    x2 = x;
                    y2 = y;
                }
            }
        }
    if ((x1 >= 1) && (x1 <= N) && (y1 >= 1) && (y1 <= M))
        move(x1, y1);
    else
        move(x2, y2);
}

void st34::revive(int& x, int& y)
{
    x = sx;
    y = sy;
}

int st34::safe(int i, int j)
{
    if (getHP() > getAtt() + 1)
        return 1;
    int sRange = getSp();
    if (sRange >= 50) sRange /= 2;
    else if (sRange >= 30) sRange = sRange * 2 / 3;
    else if (sRange >= 12) sRange = sRange * 3 / 4;
    int res = 1;
    for (int x = 1; x <= N; ++x)
        for (int y = 1; y <= M; ++y)
            if ((askWhat(x, y) != EMPTY) && (askWhat(x, y) != FOOD) && (x != getX()) && (y != getY()) && (dis(x, y, i, j) <= sRange))
                ++res;
    return res;
}

int st34::ePoint(int nx, int ny, int& dx, int& dy, int& da)
{
    int ta = 0, maxv = -1;
    dx = -1;
    dy = -1;
    da = -1;
    for (int x = 1; x <= N; ++x)
        for (int y = 1; y <= M; ++y)
            if ((dis(x, y, nx, ny) <= getSp()) && (askWhat(x, y) == EMPTY))
            {
                int value = evaluate(x, y, ta);
                if (value > maxv)
                {
                    maxv = value;
                    dx = x;
                    dy = y;
                    da = ta;
                }
            }
    return maxv;
}

int st34::evaluate(int x, int y, int& da)
{
    int sv = safe(x, y);
    int maxv = -1;
    da = -1;
    for (int t = 0; t != 4; ++t)
    {
        int x0 = x + d[t][0], y0 = y + d[t][1];
        if ((x0 >= 1) && (x0 <= N) && (y0 >= 1) && (y0 <= M))
        {
            int value = -sv;
            int target = askWhat(x0, y0);
            if ((target == FOOD) && ((getHP() <= getMaxHP() / 2) || (getExp() + 1 >= nextLevelExp())))
                value += 100;
            else if ((target != FOOD) && (target != EMPTY) && (askHP(target) <= getAtt()))
                value += 60;
            else if (target == FOOD)
                value += 35;
            if (value > maxv)
            {
                maxv = value;
                da = t;
            }
        }
    }
    return maxv;
}

void st34::increasePoint()
{
    if (getHP() <= getAtt() * 4 / 3)
        increaseHealth();
    else if (getAtt() <= getLevel() / 3)
        increaseStrength();
    else if (getMaxHP() <= getLevel() * 3 / 2)
        increaseHealth();
    else if (getSp() < (N + M - 2) * 4 / 7)
        increaseSpeed();
    else if (getAtt() <= getMaxHP() / 2)
        increaseStrength();
    else increaseHealth();
}
