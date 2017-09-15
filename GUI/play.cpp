#include "mapblock.h"
#include "mainwindow.h"
#include <cstdlib>
#include "time.h"
#include "fish.h"
#include <QTimer>
#include <algorithm>
#include "AI.h"
#include "st05.h"
#include "st06.h"
#include "st08.h"
#include "st10.h"
#include "st11.h"
#include "st12.h"
#include "st13.h"
#include "st14.h"
#include "st15.h"
#include "st16.h"
#include "st19.h"
#include "st34.h"
#include "st35.h"
#include "st02.h"
#include "st01.h"
#include <QMessageBox>


extern QLabel* plturn;
extern MainWindow* pMW;
extern mapblock* mapyg[N+1][M+1];
extern int systurn;
extern playeryg* dat[MAX_PLAYER+1];
extern std::vector<playeryg*> deadlist;
extern sys* syst;
extern QLabel* pltest;
extern std::vector<fish*> playlist;
fish* ins[MAX_PLAYER+1];
playeryg* tmp[MAX_PLAYER+1];
int aicount;
int nowid;
extern int totalplayer;
extern int lmx,lmy,mx,my;
extern int laattx,laatty;
extern int gametype;
extern int PLAYTURN;
extern int TIME_STEP;
extern int TIME_TURN;
extern int stopping;

bool MainWindow::cmp(playeryg* a,playeryg* b)
{
    if(a->sp>b->sp)return true;
    if(a->sp<b->sp)return false;
    else
    {
        if(double(a->hp)/a->maxhp>double(b->hp)/b->maxhp)return true;
        if(double(a->hp)/a->maxhp<double(b->hp)/b->maxhp)return false;
        else
        {
            if(a->score>=b->score)return true;
            else return false;
        }
    }
}
bool MainWindow::cmplast(playeryg* a,playeryg* b)
{
    if(a->score>=b->score)return true;
    else return false;
}

void MainWindow::setupai()//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
{
    /*for(int j=1;j<=5;j++)
    {
        fish* p = new st13;
        p->name = "st13";
        playlist.push_back(p);
    }
    for(int j=1;j<=6;j++)
    {
        fish* p = new st16;
        p->name = "st16";
        playlist.push_back(p);
    }*/
    for(int j=1;j<=10;j++)
    {
        fish* p = new st10;
        p->name = "st10";
        playlist.push_back(p);
    }
    for(int j=1;j<=10;j++)
    {
        fish* p = new st02;
        p->name = "st02";
        playlist.push_back(p);
    }
    for(int j=1;j<=10;j++)
    {
        fish* p = new st01;
        p->name = "st01";
        playlist.push_back(p);
    }
    /*for(int j=1;j<=1;j++)
    {
        fish* p = new st08;
        p->name = "st08";
        playlist.push_back(p);
    }
    for(int j=1;j<=10;j++)
    {
        fish* p = new st05;
        p->name = "st05";
        playlist.push_back(p);
    }
    for(int j=1;j<=5;j++)
    {
        fish* p = new st19;
        p->name = "st19";
        playlist.push_back(p);
    }
    for(int j=1;j<=1;j++)
    {
        fish* p = new st14;
        p->name = "st14";
        playlist.push_back(p);
    }*/



}

void MainWindow::init()
{
    for(int i=1;i<=N;i++)
    {
        for(int j=1;j<=M;j++)
        {
            mapblock* p = new sea;
            mapyg[i][j] = p;
        }
    }
    randalgae();

    srand((unsigned) time(NULL));

    setupai();
    totalplayer=playlist.size();

    int iid[MAX_PLAYER+1];
    for(int i=1;i<=totalplayer;i++)
    {
        iid[i]=i;
    }
    std::random_shuffle(iid+1,iid+totalplayer+1);

    for(int i=1;i<=totalplayer;i++)
    {
        fish* p = playlist[i-1];


        ins[iid[i]] = p;
        p->setID(iid[i]);
        while(1)
        {
            int rnd = rand()%(N*M);
            if(mapyg[rnd%N+1][rnd/N+1]->add=="sea")
            {
                playeryg* q = new playeryg(iid[i],rnd%N+1,rnd/N+1,p->name);
                mapyg[rnd%N+1][rnd/N+1] = q;
                dat[iid[i]] = q;
                break;
            }
        }
        p->init();
        if(dat[iid[i]]->hp==0)
        {
            deadlist.push_back(dat[iid[i]]);
            dat[iid[i]]->dead = DEADTURN;
            dat[iid[i]]->deadcount++;
            mapblock* q = new sea;
            mapyg[dat[iid[i]]->x][dat[iid[i]]->y] = q;
        }
    }
}

void MainWindow::randalgae()
{
    srand((unsigned) time(NULL));
    for(int i=1;i<=N;i++)
    {
        for(int j=1;j<=M;j++)
        {
            if(mapyg[i][j]->add=="algae")
            {
                mapblock* p = new sea;
                mapyg[i][j] = p;
            }
        }
    }
    for(int i=1;i<=MAX_FOOD;i++)
    {
        while(1)
        {

            int rnd = rand()%(N*M);
            if(mapyg[rnd%N+1][rnd/N+1]->add=="sea")
            {
                mapblock* p = new algae;
                mapyg[rnd%N+1][rnd/N+1]=p;
                break;
            }
        }
    }
}
void MainWindow::aiplay()
{
    if(aicount==totalplayer+1)
    {
        replay();
        return;
    }

    if(tmp[aicount]->dead==0)
    {
        lmx = 0;
        lmy = 0;
        mx = 0;
        my = 0;
        laattx = 0;
        laatty = 0;
        nowid = tmp[aicount]->id;
        tmp[aicount]->hasmoved=false;
        tmp[aicount]->hasattacked=false;
        ins[tmp[aicount]->id]->play();
        pMW->refresh();
        aicount++;
        if(stopping==0)
        {
            QTimer::singleShot(TIME_STEP,pMW,SLOT(aiplay()));
        }

    }
    else
    {
        aicount++;
        aiplay();
    }

}

void MainWindow::play()
{
    if(gametype==1)
    {
        plturn->setText("Turn  :   "+QString::number(systurn)+"    GameType--Rapid");
    }
    else
    {
        plturn->setText("Turn  :   "+QString::number(systurn)+"    GameType--Normal");
    }


    if(systurn%5==0)randalgae();

    for(unsigned int i=0;i<deadlist.size();++i)
    {
        deadlist[i]->dead--;
    }

    while(deadlist.size()>0&&deadlist[0]->dead==0)
    {
            if(deadlist[0]->maxhp==0)
            {
                deadlist.push_back(deadlist[0]);
                deadlist[0]->dead = DEADTURN;
                deadlist[0]->deadcount++;
            }
            else
            {
                int x,y;
                x=0;
                y=0;
                ins[deadlist[0]->id]->revive(x,y);
                if(x>0&&x<=N&&y>0&&y<=M&&mapyg[x][y]->add=="sea")
                {
                    mapyg[x][y] = deadlist[0];
                    mapyg[x][y]->x = x;
                    mapyg[x][y]->y = y;
                    deadlist[0]->hp = std::max(deadlist[0]->maxhp/10,1);
                }
                else
                {
                    srand((unsigned) time(NULL));
                    while(1)
                    {

                        int rnd = rand()%(N*M);
                        if(mapyg[rnd%N+1][rnd/N+1]->add=="sea")
                        {
                            mapyg[rnd%N+1][rnd/N+1] = deadlist[0];
                            mapyg[rnd%N+1][rnd/N+1]->x = rnd%N+1;
                            mapyg[rnd%N+1][rnd/N+1]->y = rnd/N+1;
                            deadlist[0]->hp = std::max(deadlist[0]->maxhp/10,1);
                            break;
                        }
                    }
                }
            }
            deadlist.erase(deadlist.begin());
    }


    for(int i=1;i<=totalplayer;++i)
    {
        tmp[i] = dat[i];
    }
    std::sort(tmp+1,tmp+totalplayer+1,cmp);

    nowid = 0;
    aicount = 1;
    aiplay();
}
void MainWindow::replay()
{
    systurn++;
    if(systurn<=PLAYTURN)
    QTimer::singleShot(TIME_TURN,pMW,SLOT(play()));
    if(systurn==PLAYTURN+1)
    {
        for(int i=1;i<=totalplayer;++i)
        {
            dat[i]->score+=dat[i]->exp;
        }
        std::sort(dat+1,dat+totalplayer+1,cmplast);
        QString s;
        s = "name  lv  atk  sp  maxhp  dead \n";
        for(int i=1;i<=totalplayer;++i)
        {
            s+=QString("%1%2%3%4%5%6 \n").arg(dat[i]->name).arg(dat[i]->lv,5).arg(dat[i]->att,5).arg(dat[i]->sp,5).arg(dat[i]->maxhp,5).arg(dat[i]->deadcount,5);
        }
        int r = QMessageBox::information(this,"results",s);
    }
}

