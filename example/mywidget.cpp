#include <mywidget.h>
#include <QPainter>
#include <QTimer>
#include <QPixmap>
mywidget::mywidget(QWidget *parent):
                   QLabel(parent)
{
    dir=2;//1-left,2-right;
    movedelayc=10;
    movedelay=10;
    movemax=2;
    movestep=0;
    move=false;
    x=100;
    y=100;
    isKey_Up = false;
    isKey_Down = false;
    isKey_Left = false;
    isKey_Right = false;
    this->setPixmap(QPixmap(":/add.png"));
    keycheck();
}

void mywidget::paintEvent(QPaintEvent *)
{
    if(move==false)
    {
        if(dir==1)
        {
            QPainter p(this);
            p.drawPixmap(x,y,32,32,QPixmap(":/stand0.png"));
        }
        else if(dir==2)
        {
            QImage img;
            img.load(":/stand0.png");
            img = img.mirrored(true, false);
            QPainter p(this);
            p.drawPixmap(x,y,32,32,QPixmap::fromImage(img));
        }


    }
    else
    {
        if(dir==1)
        {
            QPainter p(this);
            p.drawPixmap(x,y,32,32,QPixmap(":/move"+QString::number(movestep)+".png"));
        }
        else if(dir==2)
        {
            QImage img;
            img.load(":/move"+QString::number(movestep)+".png");
            img = img.mirrored(true, false);
            QPainter p(this);
            p.drawPixmap(x,y,32,32,QPixmap::fromImage(img));
        }



    }

}

void mywidget::keyPressEvent(QKeyEvent *k)
{
    if(k->key()==Qt::Key_Up)
    {
        isKey_Up=true;
    }
    if(k->key()==Qt::Key_Down)
    {
        isKey_Down=true;
    }
    if(k->key()==Qt::Key_Left)
    {
        isKey_Left=true;
    }
    if(k->key()==Qt::Key_Right)
    {
        isKey_Right=true;
    }

}
void mywidget::keyReleaseEvent(QKeyEvent *k)
{
    if(k->key()==Qt::Key_Up)
    {
        isKey_Up=false;
    }
    if(k->key()==Qt::Key_Down)
    {
        isKey_Down=false;
    }
    if(k->key()==Qt::Key_Left)
    {
        isKey_Left=false;
    }
    if(k->key()==Qt::Key_Right)
    {
        isKey_Right=false;
    }
}
void mywidget::keycheck()
{
    if(isKey_Up||isKey_Left||isKey_Right||isKey_Down)
    {
        move=true;
        movedelay--;
        if(movedelay==0)
        {
            if(movestep<movemax)movestep++;
            else
            {
                movestep=0;
            }
            movedelay=movedelayc;
        }
    }
    if(isKey_Up&&isKey_Left)
    {
        if(dir==1)
        {
            x--;
            y--;
        }
        else dir=1;

    }
    else if(isKey_Up&&isKey_Right)
    {
        if(dir==2)
        {
            x++;
            y--;
        }
        else dir=2;
    }
    else if(isKey_Down&&isKey_Left)
    {
        if(dir==1)
        {
            x--;
            y++;
        }
        else dir=1;
    }
    else if(isKey_Down&&isKey_Right)
    {
        if(dir==2)
        {
            x++;
            y++;
        }
        else dir=2;
    }
    else if(isKey_Up)
    {
        y--;
    }
    else if(isKey_Left)
    {
        if(dir==1)
        {
            x--;
        }
        else dir=1;
    }
    else if(isKey_Down)
    {
        y++;
    }
    else if(isKey_Right)
    {
        if(dir==2)
        {
            x++;
        }
        else dir=2;
    }
    else
    {
        move=false;
        movestep=0;
    }

    this->update();
    QTimer::singleShot(10,this,SLOT(keycheck()));
}
