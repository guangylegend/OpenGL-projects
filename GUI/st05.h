#pragma once
#ifndef H_PLAYER_ST05
#define H_PLAYER_ST05

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <cstring>
#include <ctime>
#include <cassert>
#include <algorithm>
#include <string>
#include <set>
#include <map>
#include <list>
#include <queue>
#include <vector>
#include <bitset>
#include <functional>

#include "fish.h"

using namespace std;

typedef class __player: public fish
{
public:
	int G[N+10][M+10];		//Map
	double eval[N+10][M+10];		//Evaluation for each cell

	inline bool exist(const int &, const int &);		//Check if (x, y) is inside the map
	inline bool exist(const pair<int, int> &);		//Check if (x, y) is inside the map
	inline bool legal(const int &, const int &);		//Check if (x, y) is an empty cell inside the map
	inline bool legal(const pair<int, int> &);		//Check if (x, y) is an empty cell inside the map

	void distributePoint();		//Distribute Point
	void randomDistribute(const double &, const double &, const double &);		//Increase one point to a random property

public:
	__player();

	void init();
	void play();
	void revive(int &, int &);
}st05;

bool __player::exist(const int &x, const int &y)		//Check if (x, y) is inside the map
{
	return x >= 1 && x <= N && y >= 1 && y <= M;
}

bool __player::exist(const pair<int, int> &a)		//Check if (x, y) is inside the map
{
	return a.first >= 1 && a.first <= N && a.second >= 1 && a.second <= M;
}

bool __player::legal(const int &x, const int &y)		//Check if (x, y) is an empty cell inside the map
{
	return x >= 1 && x <= N && y >= 1 && y <= M && G[x][y] == EMPTY;
}

bool __player::legal(const pair<int, int> &a)		//Check if (x, y) is an empty cell inside the map
{
	return a.first >= 1 && a.first <= N && a.second >= 1 && a.second <= M && G[a.first][a.second] == EMPTY;
}

void __player::distributePoint()		//Distribute Point
{
	while (getPoint())
	{
		if (getMaxHP() <= 0)
			randomDistribute(1.0, 0.0, 0.0);
		else if (getSp() <= 0)
			randomDistribute(0.0, 1.0, 0.0);
		else if (getAtt() <= 0)
			randomDistribute(0.0, 0.0, 1.0);

		else if (getMaxHP() <= 10)
			randomDistribute(0.5, 0.3, 0.2);
		else if (getMaxHP() <= 20)
			randomDistribute(0.4, 0.4, 0.2);
		else if (getSp() <= (N+M) >> 3)
			randomDistribute(0.2, 0.5, 0.3);
		else if (getAtt() <= 20)
			randomDistribute(0.2, 0.2, 0.6);
		else if (getSp() <= (N+M) >> 2)
			randomDistribute(0.2, 0.6, 0.2);
		else if (getSp() <= (N+M) >> 1)
			randomDistribute(0.5, 0.2, 0.3);
		else if (getSp() <= N+M-1)
			randomDistribute(0.5, 0.2, 0.3);
		else
			randomDistribute(0.3, 0.0, 0.7);
	}
}

void __player::randomDistribute(const double & p0, const double & p1, const double & p2)		//Increase one point to a random property
{
	double k = rand() % 10001/10000.0;
	if ((k -= p0) <= 0.0)
		increaseHealth();
	else if ((k -= p1) <= 0.0)
		increaseSpeed();
	else
		increaseStrength();
}

__player::__player():fish()
{
}

void __player::init()
{
	distributePoint();

	for (int k = rand() % 3; getPoint(); k = rand() % 3)
		if (k == 0)
			increaseHealth();
		else if (k == 1)
			increaseSpeed();
		else if (k == 2)
			increaseStrength();
}

void __player::play()
{
	const int ox[] = {0, 0, -1, 1}, oy[] = {-1, 1, 0, 0};

	distributePoint();

	for (int i = 1; i <= N; ++i)
		for (int j = 1; j <= M; ++j)
			G[i][j] = askWhat(i, j);

	memset(eval, 0, sizeof(eval));

	for (int i = 1; i <= N; ++i)
		for (int j = 1; j <= M; ++j)
			if (G[i][j] != EMPTY && (i != getX() || j != getY()))
				if (G[i][j] == FOOD)
				{
					for (int k = 0; k < 4; ++k)
						if (exist(i+ox[k], j+oy[k]))
							if (getHP() <= 10)
								eval[i+ox[k]][j+oy[k]] += 15.0;
							else if (getHP() <= 20)
								eval[i+ox[k]][j+oy[k]] += 10.0;
							else
								eval[i+ox[k]][j+oy[k]] += 5.0;
				}
				else
				{
					int dist = abs(i-getX())+abs(j-getY());
					if (dist-1 <= getSp())
						if (askHP(G[i][j]) <= getAtt())
						{
							for (int k = 0; k < 4; ++k)
								if (exist(i+ox[k], j+oy[k]))
									eval[i+ox[k]][j+oy[k]] += 20.0;
						}
						else
						{
							for (int k = 0; k < 4; ++k)
								if (exist(i+ox[k], j+oy[k]))
									eval[i+ox[k]][j+oy[k]] += 3.0;
						}
					else
					{
							for (int k = 0; k < 4; ++k)
								if (exist(i+ox[k], j+oy[k]))
									eval[i+ox[k]][j+oy[k]] += 4.0;
					}
				}
	/*
	for (int i = 1; i <= N; ++i)
		for (int j = 1; j <= M; ++j)
			if (G[i][j] != EMPTY && G[i][j] != FOOD && (i != getX() || j != getY()))
			{
				if (getHP() < askHP(G[i][j]))
				{
					for (int k = 1; k <= N; ++k)
						for (int l = 1; l <= M; ++l)
							eval[i][j] -= 4.0/(abs(k-i)+abs(k-j)+1);
				}
				else
				{
					for (int k = 1; k <= N; ++k)
						for (int l = 1; l <= M; ++l)
							eval[i][j] -= 1.0/(abs(k-i)+abs(k-j)+3);
				}
			}
	*/
	vector<pair<double, pair<int, int> > > sortEval;
	sortEval.clear();
	for (int i = 1; i <= N; ++i)
		for (int j = 1; j <= M; ++j)
			sortEval.push_back(make_pair(eval[i][j]+rand() % 10000/1e5, make_pair(i, j)));
	sort(sortEval.begin(), sortEval.end(), greater<pair<double, pair<int, int> > >());
	for (vector<pair<double, pair<int, int> > >::iterator iter = sortEval.begin(); iter != sortEval.end(); ++iter)
		if (legal(iter -> second) && abs((iter -> second).first-getX())+abs((iter -> second).second-getY()) <= getSp())
		{
			if (abs((iter -> second).first-getX())+abs((iter -> second).second-getY()) <= getSp())
				move((iter -> second).first, (iter -> second).second);
			else
			{
				pair<int, int> t = make_pair((iter -> second).first, (iter -> second).second);
				double k = (double)getSp()/(abs(t.first-getX())+abs(t.second-getY()));
				t = make_pair(getX()+(int)floor((t.first-getX())*k), getY()+(int)floor((t.second-getY())*k));
				move(t.first, t.second);
			}
			break;
		}

	vector<pair<double, int> > attackEval;
	attackEval.clear();
	for (int i = 0, t; i < 4; ++i)
		if (exist(getX()+ox[i], getY()+oy[i]))
		{
			t = G[getX()+ox[i]][getY()+oy[i]];
			if (t != EMPTY)
				if (t == FOOD)
					attackEval.push_back(make_pair(5.0, i));
				else
					if (askHP(t) <= getAtt())
						attackEval.push_back(make_pair(10.0*askHP(t), i));
					else
                        attackEval.push_back(make_pair(5.0-atan(1.*askHP(t)), i));
		}
	sort(attackEval.begin(), attackEval.end(), greater<pair<double, int> >());
	if (attackEval.size())
		attack(getX()+ox[attackEval.begin() -> second], getY()+oy[attackEval.begin() -> second]);
}

void __player::revive(int &x, int &y)
{
	x = rand() % N+1;
	y = rand() % M+1;
}

#endif
