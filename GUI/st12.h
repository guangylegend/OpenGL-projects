/*
	edly

	Just have fun
*/

#pragma once

#include <cstdlib>
#include <vector>

#include "fish.h"

class st12:
	virtual public fish
{
	private:
		int go0[4];
		int go1[4];
		int go2[4];
		int my_hp;
		int add_point_time;
		int dead_time;
		int map[50][50];
		int map_hp[50][50];
		int v[50][50];
		void add_dps();
		void add_hp();
		void add_speed();
		int dis(int,int,int,int);
		int random(int);
		void work(int);
		void funny_add_point();
		void add_point(double,double,double);

		struct attack_able
		{
			int x,y,xx,yy,map,map_hp;
			attack_able(int a,int b,int c,int d,int e,int f)
			{
				x=a; y=b; xx=c; yy=d; map=e; map_hp=f;
			}
			bool operator <(attack_able b) const
			{
				return map_hp>b.map_hp;
			}
		};

		std::vector<attack_able> attack_able_vec;

	public:
		void init();
		void play();
		void revive(int&,int&);
};
