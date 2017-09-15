#include <QtGui/QApplication>
#include "mainwindow.h"
#include "mapblock.h"
#include "sys.h"
#include "fish.h"
#include <QMessageBox>

mapblock* mapyg[N+1][M+1];
MainWindow* pMW;
playeryg* dat[MAX_PLAYER+1];
std::vector<playeryg*> deadlist;
std::vector<fish*> playlist;
sys* syst;
int systurn;
int lmx,lmy,mx,my;
int laattx,laatty;
int gametype = 0;
int PLAYTURN=500;
int TIME_STEP=1000;
int TIME_TURN=0;
int stopping=0;
int totalplayer=0;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    pMW = &w;
    sys* syst = new sys;
    systurn=1;
    lmx=0;
    lmy=0;
    mx=0;
    my=0;
    laattx=0;
    laatty=0;
    deadlist.clear();
    playlist.clear();
    pMW->init();
    int r = QMessageBox::warning(pMW,"mode select","Do you want to open rapid mode?",QMessageBox::Yes|QMessageBox::No);
    if(r == QMessageBox::Yes)
    {
        gametype = 1;
        TIME_STEP = 0;
    }
    w.show();

    pMW->play();

    return a.exec();
}
