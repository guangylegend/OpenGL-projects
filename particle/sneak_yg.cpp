#include "sneak_yg.h"

Spark::Spark() : initRandom(1) , initVelocity(10) , timeTrace(0.01) , x(0) , y(0) , z(0) , vX(0) , vY(0) , vZ(0)
{
}
void Spark::init()
{
    z = 0 ;
    x = rand() % 100 / 100.0 * initRandom ;
    y = rand() % 100 / 100.0 * initRandom ;

    vX = rand() % 100 / 100.0 * initVelocity - initVelocity/2 ;
    vY = rand() % 100 / 100.0 * initVelocity - initVelocity/2 ;
    vZ = rand() % 100 / 100.0 * initVelocity + sqrt(vX*vX+vY*vY) + initVelocity ;

    acX = 0 ;
    acY = 0 ;
    acZ = -9.8 ;

    red = rand()%256 ;
    green = rand()%256 ;
    blue = rand()%256;
    timePeriod = rand() % 1000 + 1000 ;
}
void Spark::move()
{
    x += vX * timeTrace ;
    y += vY * timeTrace ;
    z += vZ * timeTrace ;
    vX += acX * timeTrace ;
    vY += acY * timeTrace ;
    vZ += acZ * timeTrace ;


    --timePeriod ;
}
void Spark::refresh()
{
    if ( timePeriod < 0 || z < 0 )
    {
        init();
    }
}
float Spark::getX()
{
    return x ;
}
float Spark::getY()
{
    return y ;
}
float Spark::getZ()
{
    return z ;
}
int Spark::getR()
{
    return red ;
}
int Spark::getG()
{
    return green ;
}
int Spark::getB()
{
    return blue ;
}


Flame::Flame() {
    size = 0 ;
    array.clear();
}
Flame::Flame( int initSize )
{
    size = initSize ;
    array.clear();
    for ( int i = 0 ; i < size ; ++i )
        array.push_back( Spark() );
}
void Flame::init()
{
    srand((int)time(NULL));
    for ( int i = 0 ; i < size ; ++i )
        array[i].init();
}
void Flame::move()
{
    for ( int i = 0 ; i < size ; ++i )
    {
        array[i].move();
        array[i].refresh();
    }
}
vector<Spark> Flame::state()
{
    return array;
}
