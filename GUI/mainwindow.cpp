#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <QPaintEvent>
#include <qpixmap>
#include "mapblock.h"
#include <QMessageBox>

extern mapblock* mapyg[N+1][M+1];
extern playeryg* dat[MAX_PLAYER+1];
extern int nowid;
extern int lmx,lmy,mx,my;
extern int laattx,laatty;
extern int gametype;
extern int stopping;
extern int totalplayer;
QLabel* plturn;
QLabel* plid;
QLabel* plhp;
QLabel* platt;
QLabel* plsp;
QLabel* pllv;
QLabel* pltest;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    plturn = ui->labelturn;
    plhp = ui->labelhp;
    platt = ui->labelatt;
    pllv = ui->labellv;
    plsp = ui->labelsp;
    plid = ui->labelid;
    pltest = ui->labeltest;
    ui->buttonrelease->hide();
    ui->pushButton_2->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::paintEvent(QPaintEvent *)
{
    if(gametype == 0)
    {
        QPainter p(this);
        for(int i=0;i<=N-1;i++)
        {
            for(int j=0;j<=M-1;j++)
            {
                if(mapyg[i+1][j+1]->add=="fish"&&mapyg[i+1][j+1]->id==nowid)
                {
                    p.drawPixmap(i*16,j*16,16,16,QPixmap(":/play.png"));
                }
                else
                {
                    p.drawPixmap(i*16,j*16,16,16,QPixmap(":/" + mapyg[i+1][j+1]->add + ".png"));
                }

            }
        }
            p.setPen(QPen(Qt::red, 5, Qt::SolidLine, Qt::RoundCap));
            if(lmx!=0)
            {
                p.drawPixmap((lmx-1)*16,(lmy-1)*16,16,16,QPixmap(":/anchor.png"));
                p.drawLine(lmx*16-8,lmy*16-8,mx*16-8,my*16-8);

            }
            if(laattx!=0)
            {
                p.drawPixmap((laattx-1)*16,(laatty-1)*16,16,16,QPixmap(":/cancel.png"));
            }
    }
}
void MainWindow::mousePressEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton)
    {
        int mx = e->x();
        int my = e->y();
        if(mx<=N*16-1&&my<=M*16-1)
        {
            if(mapyg[mx/16+1][my/16+1]->add=="fish")
            {
                plid->setText("ID:    "+QString::number(mapyg[mx/16+1][my/16+1]->id)+"("+mapyg[mx/16+1][my/16+1]->name+")");
                plhp->setText("HP:    "+QString::number(mapyg[mx/16+1][my/16+1]->hp)+"/"+QString::number(mapyg[mx/16+1][my/16+1]->maxhp));
                pllv->setText("LV:    "+QString::number(mapyg[mx/16+1][my/16+1]->lv));
                platt->setText("ATK:    "+QString::number(mapyg[mx/16+1][my/16+1]->att));
                plsp->setText("SP:    "+QString::number(mapyg[mx/16+1][my/16+1]->sp));
                pltest->setText("DEAD:    "+QString::number(mapyg[mx/16+1][my/16+1]->deadcount));
            }

        }
    }
}

void MainWindow::refresh()
{
    this->update();
}

void MainWindow::textout(QString s)
{
    ui->labelout5->setText(ui->labelout4->text());
    ui->labelout4->setText(ui->labelout3->text());
    ui->labelout3->setText(ui->labelout2->text());
    ui->labelout2->setText(ui->labelout1->text());
    ui->labelout1->setText(s);
}


void MainWindow::on_buttonstop_clicked()
{
    stopping = 1;
    ui->buttonrelease->show();
    ui->pushButton_2->show();
    ui->buttonstop->setEnabled(false);
}

void MainWindow::on_buttonrelease_clicked()
{
    stopping = 0;
    ui->buttonstop->setEnabled(true);
    ui->buttonrelease->hide();
    ui->pushButton_2->hide();
    aiplay();
}

void MainWindow::on_pushButton_2_clicked()
{
    for(int i=1;i<=totalplayer;++i)
    {
        dat[i]->score+=dat[i]->exp;
    }
    std::sort(dat+1,dat+totalplayer+1,cmplast);
    QString s;
    s = "name  lv  atk  sp   hp  dead \n";
    for(int i=1;i<=totalplayer;++i)
    {
        s+=QString("%1%2%3%4%5%6 \n").arg(dat[i]->name).arg(dat[i]->lv,5).arg(dat[i]->att,5).arg(dat[i]->sp,5).arg(dat[i]->maxhp,5).arg(dat[i]->deadcount,5);
    }
    int r = QMessageBox::information(this,"results",s);
}
