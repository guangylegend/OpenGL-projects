#include "st02.h"
#include <cstdio>
#include <algorithm>
#include <cmath>
//#define PLAYER_DEBUG 
void st02::init(){
    increaseStrength();
	while(getPoint()){
		if(getPoint())increaseHealth();
		if(getPoint())increaseSpeed();
	}
	dir[0][0] = 1 ;
	dir[0][1] = 0 ;
	dir[1][0] = -1 ;
	dir[1][1] = 0 ;
	dir[2][0] = 0 ;
	dir[2][1] = 1 ;
	dir[3][0] = 0 ;
	dir[3][1] = -1 ;
	// my init
	counting = 0 ;
	me.ID = askWhat( getX() , getY() );
}
//  the best position i can go
void st02::choose_position( int &x , int &y )
{
    int inf = 1000000000;
    double maxs = -inf ;
    for ( int i = 1 ; i <= N ; ++i )
        for ( int j = 1 ; j <= M ; ++j )
            if ( map[i][j] == EMPTY )
                if ( abso( i - me.xcoor ) + abso( j - me.ycoor ) <= me.speed )
                {
                    if ( maxs < value_to_move[i][j] )
                    {
                        x = i ;
                        y = j ;
                        maxs = value_to_move[i][j] ;
                    }
                }
}
void st02::Attack_action()
{
#ifdef PLAYER_DEBUG
printf("Attack action\n");
#endif
    for ( int l = 0 ; l < 4 ; ++l )
    {
        int xx = me.xcoor+dir[l][0] , yy = me.ycoor+dir[l][1] ;
        if ( inside( xx , yy ) )
        if ( askWhat(xx,yy) != FOOD && askWhat(xx,yy) != EMPTY )
        if ( me.attk >= askHP( askWhat(xx,yy) ) )
        {
            #ifdef PLAYER_DEBUG
            printf("%d %d ----- %d %d : %d %d\n",me.xcoor,me.ycoor,xx,yy,dir[l][0],dir[l][1]);
            #endif
            attack(xx,yy);
            return ;
        }
    }
    for ( int l = 0 ; l < 4; ++l )
    {
        int xx = me.xcoor+dir[l][0] , yy = me.ycoor+dir[l][1] ;
        if ( inside(xx,yy) )
        if ( askWhat(xx,yy) == FOOD )
        {
            #ifdef PLAYER_DEBUG
            printf("%d %d ----- %d %d : %d %d\n",me.xcoor,me.ycoor,xx,yy,dir[l][0],dir[l][1]);
            #endif
            attack(xx,yy);
            return ;
        }
    }
    for ( int l = 0 ; l < 4 ; ++l )
    {
        int xx = me.xcoor+dir[l][0] , yy = me.ycoor+dir[l][1] ;
        if ( inside( xx , yy ) )
        if ( askWhat(xx,yy) != FOOD && askWhat(xx,yy) != EMPTY )
        {
            #ifdef PLAYER_DEBUG
            printf("%d %d ----- %d %d : %d %d\n",me.xcoor,me.ycoor,xx,yy,dir[l][0],dir[l][1]);
            #endif
            attack(xx,yy);
            return ;
        }
    }
    return ;
}
void st02::play()
{
    ++counting;
    Map_Update();
	MyInformation_Update();
	MyPoint_assign();
	EnemyInformation_Update();
	Value_Assess();
	
	int newx = -1 , newy = -1  ;
    
    choose_position( newx , newy );
    
    if ( newx != -1 )
    {
        move( newx , newy ) ;
        me.xcoor = newx ;
        me.ycoor = newy ;
    }
    
    Attack_action(  );
    
#ifdef PLAYER_DEBUG
printf("attack complete\n");
#endif
    MyInformation_Update();
    MyPoint_assign();
}

void st02::revive(int& x,int& y)
{
    counting += 2 ;
    me.deadtime++;
    
    MyInformation_Update();
    EnemyInformation_Update();
    Value_Assess();
    
    x = -1 , y = -1 ;
    int t = me.speed ;
    me.speed = (N+M) * 10 ;
    choose_position( x , y ) ;
    me.speed = t ;
    return ;
}
//  ------------------------------------------------    //
//  enemy update
//  after map update

void st02::EnemyInformation_Update()
{
    for ( int i = 1 ; i <= N ; ++i )
        for ( int j = 1 ; j <= M ; ++j )
        if ( map[i][j] > 0 && map[i][j] != me.ID )
        {
            int u = map[i][j] ;
            enemy_hp[u] = askHP( u );
            enemy_maxhp[u] = max( enemy_hp[u] , enemy_maxhp[u] ) ;
            enemy_speed[u] = max( enemy_speed[u] , abso(enemy_coor[u][0] - i) + abso(enemy_coor[u][1] - j) );
            enemy_attk[u] = max( max(max( enemy_attk[u] , counting/25 ) , std::min(enemy_speed[u],counting/10-enemy_speed[u]-2*enemy_maxhp[u]) ) , std::min(enemy_maxhp[u],counting/10-enemy_speed[u]-2*enemy_maxhp[u]) );
            enemy_level[u] = counting/10;
            enemy_coor[u][0] = i ;
            enemy_coor[u][1] = j ;
        }
}
//  ------------------------------------------------    //
//  Map update
void st02::Map_Update()
{
    for ( int i = 1 ; i <= N ; ++i )
        for ( int j = 1 ; j <= M ; ++j )
            map[i][j] = askWhat(i,j);
    
    EnemyInformation_Update();
}
//  -----------------------------------------------     //
//  my update
void st02::MyInformation_Update()
{
    me.level = getLevel();//返回当前玩家等级
    me.exp = getExp();//返回当前玩家经验
    me.xcoor = getX();//返回当前玩家横坐标
    me.ycoor = getY();//返回当前玩家纵坐标
    me.hp = getHP();//返回当前玩家生命值
    me.maxhp = getMaxHP();//返回当前玩家生命上限
    me.attk = getAtt();//返回当前玩家攻击力
    me.speed = getSp();//返回当前玩家速度}
}
//  -------------------------------------------------   //
//  Healthy , speed  update
void st02::MyPoint_assign()
{
    while( getPoint() )
    {
        bool fl = true;
        if ( getPoint() && Necessary_attk() )
        {
            fl = false ;
            increaseStrength() ;
            me.attk++;
        }
        if( getPoint() && Necessary_hp() )
        {
            fl = false ;
            increaseHealth();
            me.hp += 2 ;
            me.maxhp += 2 ;
        }
        if( getPoint() && Necessary_speed() )
        {
            fl = false ;
            increaseSpeed();
            me.speed ++ ;
        }
        if ( fl )
            break;
    }
}
//  --------------------------------------------------- //
//  judge if need hp update
bool st02::Necessary_hp()
{
//    return me.hp <= me.attk*5;
    int maxhp = 0 ;
    for ( int i = 1 ; i <= getTotalPlayer() ; ++i )
        maxhp = std::max( maxhp , enemy_maxhp[i] );
    return me.hp <= max( maxhp , me.attk*5 );
}
//  --------------------------------------------------- //
//  judge if need hp update
bool st02::Necessary_attk()
{
    int seq[MAX_PLAYER+1] ;
    int top = 0 ;
    for ( int i ; i <= getTotalPlayer() ; ++i )
    {
        seq[++top] = enemy_maxhp[i] ;
        if ( enemy_maxhp[i] == 0 )
            --top ;
    }
    std::sort( seq+1 , seq+top+1 );
    
    if ( counting <= 50 )
        return me.attk < seq[top*3/4] ;
    else if ( counting <= 100 )
        return me.attk < seq[top*4/5] ;
    else if ( counting <= 150 )
        return me.attk < seq[top*7/8] ;
    else if ( counting <= 250 )
        return me.attk < seq[top*9/10] ;
    else
        return me.attk < seq[top] ;
}
//  --------------------------------------------------- //
//  judge if need speed update
bool st02::Necessary_speed()
{
    if ( me.speed <= 10 )
        return true ;
    if ( me.speed <= 20 && counting >= me.speed * 3 )
        return true ;
    return me.speed <= std::min( counting/5 , N+M );
}
//  --------------------------------------------------- //
//  Find number of Food I can reach in speed
int st02::Find_Food( int addition )
{
    int res = 0 ;
    for ( int i = 1 ; i <= N ; ++i )
        for ( int j = 1 ; j <= M ; ++j )
            if ( abso( me.xcoor-i ) + abso( me.ycoor-j ) <= me.speed+addition )
                if ( map[i][j] == FOOD )
                    res ++;
    return res ;
}
//  --------------------------------------------------- //
//  Previous value Assess
void st02::Previous_Assess()
{
    int c1 , c2 , c3 ;
    int inf = 100000;
    
    c1 = me.speed ;
    c2 = inf ;
    c3 = inf ;
    
    if ( me.hp <= me.maxhp * 4 / 10 )
        c2 = 10 ;
    else if ( me.hp <= me.maxhp * 6.5 / 10 )
        c2 = c1 ;
    
    for ( int i = 1 ; i <= N ; ++i )
        for ( int j = 1 ; j <= M ; ++j )
        {
            if ( map[i][j] == FOOD )
            {
                for ( int x = 1 ; x <= N ; ++x )
                    for ( int y = 1 ; y <= N ; ++y )
                        if ( abso(x-i) + abso(y-j) <= me.speed )
                            value_to_move[x][y] += c1 - abso(x-i) - abso(y-j) ;
                for ( int l = 0 ; l < 4 ; ++l )
                    if ( inside( i + dir[l][0] , j + dir[l][1] ) )
                        value_to_move[i+dir[l][0]][j+dir[l][1]] += c1*10 ;
                value_to_move[i][j] += c1*10 ;
            }
            else if ( map[i][j] != EMPTY && map[i][j] != FOOD && map[i][j] != me.ID )
            {
                if ( enemy_hp[ map[i][j] ] <= me.attk )
                {
                    for ( int l = 0 ; l < 4 ; ++l )
                        if ( inside( i + dir[l][0] , j + dir[l][1] ) )
                            value_to_move[i+dir[l][0]][j+dir[l][1]] += c2 * enemy_hp[ map[i][j] ]  ;
                    value_to_move[i][j] += c2 * enemy_hp[ map[i][j] ]  ;
                }
                else if ( enemy_hp[ map[i][j] ] <= me.attk*1.5 )
                {
                    for ( int l = 0 ; l < 4 ; ++l )
                        if ( inside( i + dir[l][0] , j + dir[l][1] ) )
                            value_to_move[i+dir[l][0]][j+dir[l][1]] += (int)pow( c2 , 1-((double)enemy_hp[map[i][j]]/me.attk-0.5) );
                    value_to_move[i][j] += (int)pow( c2 , 1-((double)enemy_hp[map[i][j]]/me.attk-0.5) );
                }
                if ( me.hp <= me.maxhp*7/10 )
                {
                    if ( enemy_hp[map[i][j]] - me.attk < me.hp )
                    {
                        for ( int l = 0 ; l < 4 ; ++l )
                        if ( inside( i + dir[l][0] , j + dir[l][1] ) )
                            value_to_move[i+dir[l][0]][j+dir[l][1]] += c3 ;
                        value_to_move[i][j] += c3*10 ;
                    }
                }
            }
        }
}
//  --------------------------------------------------- //
//  count value Assess
void st02::Value_Assess()
{
    for ( int i = 1 ; i <= N ; ++i )
        for ( int j = 1 ; j <= M ; ++j )
            value_to_move[i][j] = 0 ;
    Previous_Assess();
}
