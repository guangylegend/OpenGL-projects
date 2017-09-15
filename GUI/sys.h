#ifndef SYS_H
#define SYS_H

class sys {
    public:
    sys();
    ~sys();
    bool move(int,int,int);//移动到某个位置
    bool attack(int,int,int);//攻击某个位置
    int getPoint(int);//返回当前玩家剩余点数
    int getLevel(int);//返回当前玩家等级
    int getExp(int);//返回当前玩家经验
    int getX(int);//返回当前玩家横坐标
    int getY(int);//返回当前玩家纵坐标
    int getHP(int);//返回当前玩家生命值
    int getMaxHP(int);//返回当前玩家生命上限
    int getAtt(int);//返回当前玩家攻击力
    int getSp(int);//返回当前玩家速度
    int askWhat(int,int);//返回地图某个位置内容（玩家ID或FOOD或EMPTY）
    int askHP(int);//询问某个玩家ID当前血量
    int getTotalPlayer();//获取玩家总人数
    bool increaseHealth(int);//增加health属性1
    bool increaseStrength(int);//增加Strength属性1
    bool increaseSpeed(int);//增加Speed属性1

};

#endif // SYS_H
