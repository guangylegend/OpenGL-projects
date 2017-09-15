#ifndef MYWIDGET_H
#define MYWIDGET_H
#include <QLabel>
#include <QKeyEvent>

class mywidget : public QLabel
{
    Q_OBJECT
    int dir;
    bool move;
    int movemax,movestep;
    int movedelayc,movedelay;
    int x,y;
    bool isKey_Down,isKey_Left,isKey_Up,isKey_Right;
public:
    mywidget(QWidget *parent = 0);

private slots:
    void keycheck();
protected:
    void keyPressEvent(QKeyEvent *);
    void keyReleaseEvent(QKeyEvent *);
    void paintEvent(QPaintEvent *);
};

#endif // MYWIDGET_H
