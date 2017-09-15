#include "st13.h"
#include <cstring>
#include <algorithm>
#define max(x,y) ((x)>(y)?(x):(y))
#define min(x,y) ((x)<(y)?(x):(y))
int st13::step[5][3]={{0,0,0},{0,-1,0},{0,0,-1},{0,1,0},{0,0,1}};

void st13::trans1(){
	for (int i=1;i<=N;++i)
		for (int j=1;j<=M;++j)
			cond1[i][j]=cond2[i][j];
}

void st13::trans2(){
	for (int i=1;i<=pnum;++i)
		allhp1[i]=allhp2[i];
}

void st13::trans3(){
	for (int i=1;i<=pnum;++i){
		allspot1[i][1]=allspot2[i][1];
		allspot1[i][2]=allspot2[i][2];
	}
}

void st13::getMap(){
	fnum=0;
	memset(allspot3,0,sizeof(allspot3));
	for (int i=1;i<=pnum;++i) allspot2[i][1]=-1;
	for (int i=1;i<=N;++i)
		for (int j=1;j<=M;++j){
			cond2[i][j]=askWhat(i,j);
			if (cond2[i][j]>0){
				allspot2[cond2[i][j]][1]=i;
				allspot2[cond2[i][j]][2]=j;
			}
			if (cond2[i][j]==FOOD){
				++fnum;
				allspot3[fnum][1]=i;
				allspot3[fnum][2]=j;
			}
		}
}

void st13::getallhp(){
	for (int i=1;i<=pnum;++i)
		allhp2[i]=askHP(i);
}

void st13::newspeed(){
	for (int i=1;i<=pnum;++i){
		int x1=allspot1[i][1],y1=allspot1[i][2],x2=allspot2[i][1],y2=allspot2[i][2];
		if ((x1==-1)||(x2==-1)) continue;
		if (allspeed[i]>=N+M-2) continue;
		if (abs(x1+y1-x2-y2)>allspeed[i])
			allspeed[i]=abs(x1+y1-x2-y2);
		else
			if ((istherefood(x2,y2)<5) &&(allhp2[i]>allhp1[i])&&(allhp2[i]-allhp1[i]<=2)) ++allspeed[i];
	}
}

void st13::getden1(){
	int xx,yy,j,p,q;
	memset(density1,0,sizeof(density1));
	for (int i=1;i<=pnum;++i){
		xx=allspot2[i][1];
		yy=allspot2[i][2];
		if (xx==-1) continue;
		j=xx-allspeed[i];
		while (j<=xx+allspeed[i]){
			if (j<1){++j;continue;}
			p=max(1,yy-allspeed[i]+abs(xx-j));
			q=min(M,yy+allspeed[i]-abs(xx-j));
			for (int k=p;k<=q;++k)
				++density1[j][k];
			++j;
			if (j>N) break;
		}
	}
}

double st13::value(int xx,int yy){
	double v=1.0,j=0.0,k;
	int f=istherefood(xx,yy);
	if (f<5) j=hpincrease()+4.0/expneed();
	int g=istherefish(xx,yy);
	if (g<5){
		k=max(1,getLevel()/2)*4.0/expneed();
		if (j<k) j=k;
	}
	v=v+j*10;
	return v/(1+density1[xx][yy]);
}

int st13::istherefood(int xx,int yy){
	int x0,y0,i=1;
	while (i<=4){
		x0=xx+step[i][1];
		y0=yy+step[i][2];
		if ((1<=x0)&&(x0<=N)&&(1<=y0)&&(y0<=M)&&(cond2[x0][y0]==FOOD)) break;
		++i;
	}
	return i;
}

int st13::istherefish(int xx,int yy){
	int x0,y0,i=1;
	while (i<=4){
		x0=xx+step[i][1];
		y0=yy+step[i][2];
		if ((1<=x0)&&(x0<=N)&&(1<=y0)&&(y0<=M)&&(cond2[x0][y0]>0)&&(cond2[x0][y0]!=getID())) break;
		++i;
	}
	if (i<5){
		int k=allhp2[cond1[xx+step[i][1]][yy+step[i][2]]];
		for (int j=i+1;j<=4;++j){
			x0=xx+step[j][1];
			y0=yy+step[j][2];
			if ((1<=x0)&&(x0<=N)&&(1<=y0)&&(y0<=M)&&(cond2[xx][yy]>0)&&(allhp2[cond1[x0][y0]]<k)) i=j;
		}
	}
	if (i<5){
		x0=xx+step[i][1];
		y0=yy+step[i][2];
		if (allhp2[cond2[x0][y0]]<=getPoint()+getAtt()) return i; else return 5;
	}
	else
		return i;
}

int st13::hpincrease(){
	return min(getMaxHP(),getHP()+max(2,getMaxHP()/10))-getHP();
}

int st13::expneed(){
	int l=getLevel();
	return (l+1)*(l+2)/2-1-getExp();
}

void st13::setPoint(int x ,int y,int z){
	for (int i=1;i<=x;++i) increaseHealth();
	for (int i=1;i<=y;++i) increaseStrength();
	for (int i=1;i<=z;++i) increaseSpeed();
}

void st13::init(){
	setPoint(5,1,4);
	getMap();
	pnum=0;
	for (int i=1;i<=N;++i)
		for (int j=1;j<=M;++j)
			if (cond2[i][j]>0) ++pnum;
	trans1();
	trans3();
	memset(allspeed,1,sizeof(allspeed));
	for (int i=1;i<=pnum;++i) allhp1[i]=20;
	justrevive=false;
}

void st13::play(){
	if (getPoint()>=3){increaseHealth(); increaseSpeed();}
	getMap();
	getallhp();
	if (!justrevive) newspeed(); else justrevive=false;
	getden1();
	px=getX();
	py=getY();
	pspeed=getSp();
	pattack=getAtt();
	php=getHP();
	int xx=px-pspeed,yy,pp,qq,bestx=0,besty=0;
	double v,max=0.0;
	while (xx<=px+pspeed){
		if (xx<1) {++xx;continue;}
		pp=max(1,py-pspeed+abs(xx-px));
		qq=min(M,py+pspeed-abs(xx-px));
		for (yy=pp;yy<=qq;++yy)
			if (cond2[xx][yy]==EMPTY){
			v=value(xx,yy);
			if (v>max){
				max=v;
				bestx=xx;
				besty=yy;
			}
			}
		++xx;
		if (xx>N) break;
	}
	move(bestx,besty);
	int j=istherefood(bestx,besty);
	if (j<5){
		xx=bestx+step[j][1];
		yy=besty+step[j][2];
		attack(xx,yy);
	}
	j=istherefish(bestx,besty);
//	cout<<j<<" ";
	if (j<5){
		xx=bestx+step[j][1];
		yy=besty+step[j][2];
		int maxatt=pattack+getPoint();
		if (allhp2[cond2[xx][yy]]<=maxatt){
			for (int j=1;j<=allhp2[cond2[xx][yy]]-pattack;++j) increaseStrength();
			attack(xx,yy);
		}
		else
			attack(xx,yy);
	}
//	cout<<endl;
	trans1();
	trans2();
	trans3();
}

void st13::revive(int& x,int& y){
	int min=200,bestx,besty,k;
	for (int i=1;i<=M;++i)
		for (int j=1;j<=N;++j){
			k=density1[i][j];
			if (k<min){
				min=k;
				bestx=i;
				besty=j;
			}
		}
	x=bestx;
	y=besty;
	justrevive=true;
}

