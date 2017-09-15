#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include "mapblock.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void refresh();

public slots:
    void init();
    void randalgae();
    void play();
    void setupai();
    void textout(QString s);
    void aiplay();
    void replay();

private:
    Ui::MainWindow *ui;
    static bool cmplast(playeryg* a,playeryg* b);
    static bool cmp(playeryg* a,playeryg* b);
protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *e);
private slots:
    void on_buttonstop_clicked();
    void on_buttonrelease_clicked();
    void on_pushButton_2_clicked();
};

#endif // MAINWINDOW_H
