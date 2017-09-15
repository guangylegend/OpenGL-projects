#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLabel>



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->label->setFocus();
    ui->label->setStyleSheet("background:transparent");
    //ui->centralWidget->setStyleSheet("background-image: url(:/accept.png)");
}

MainWindow::~MainWindow()
{
    delete ui;
}


