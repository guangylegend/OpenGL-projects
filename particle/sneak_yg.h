#ifndef _sneak_yg
#define _sneak_yg

#include <ctime>
#include <cstdlib>
#include <cmath>
#include <vector>
using namespace std ;

class Spark
{
    float initRandom , initVelocity ;   //  const
    float timeTrace ;   //  const
    float timePeriod ;
    
    float x , y , z ;
    float vX , vY , vZ ;
    float acX , acY , acZ ;
    
    int red , green , blue ;
    
    public:
        Spark();
        void init() ;
        void move() ;
        void refresh() ;
        float getX() ;
        float getY() ;
        float getZ() ;
        int getR();
        int getG();
        int getB();
};

class Flame
{
    int size ;
    vector<Spark> array ;
    
    public:
    Flame();
    Flame(int);
    void init() ;
    void move() ;
    vector<Spark> state() ;
};
#endif
