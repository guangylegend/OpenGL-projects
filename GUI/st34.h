#include "fish.h"
#include <cstdlib>

class st34 : public fish
{
public:

    void init();

    void play();

    void revive(int&, int&);

private:

    int safe(int, int);

    int evaluate(int, int, int&);

    int ePoint(int, int, int&, int&, int&);

    void increasePoint();

    inline int dis(int x1, int y1, int x2, int y2)
    {
        return (std::abs(x1 - x2) + std::abs(y1 - y2));
    }

    inline int dis(int x, int y)
    {
        return dis(x, y, getX(), getY());
    }

    inline int nextLevelExp()
    {
        return (2 + getLevel() + 1) * getLevel() / 2;
    }

    static const int MAX = 10000;

    int d[4][2];

    int sx;

    int sy;
};

//�������������躯�����躯�������������豣֤����ÿ��������������ɵ���

//��Ҫʹ��ȫ�ֱ�������
