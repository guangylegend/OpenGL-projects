/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Tue Dec 11 17:33:25 2012
**      by: Qt User Interface Compiler version 4.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QLabel *labelturn;
    QLabel *labelback;
    QLabel *labelid;
    QLabel *labellv;
    QLabel *labelhp;
    QLabel *labelatt;
    QLabel *labelsp;
    QLabel *labeltest;
    QLabel *labelout1;
    QLabel *labelout3;
    QLabel *labelout5;
    QLabel *labelout4;
    QLabel *labelout2;
    QPushButton *buttonstop;
    QPushButton *pushButton_2;
    QPushButton *buttonrelease;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1000, 640);
        MainWindow->setAutoFillBackground(false);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        labelturn = new QLabel(centralWidget);
        labelturn->setObjectName(QString::fromUtf8("labelturn"));
        labelturn->setGeometry(QRect(650, 20, 420, 30));
        labelturn->setStyleSheet(QString::fromUtf8("font: 75 14pt \"Aharoni\";"));
        labelback = new QLabel(centralWidget);
        labelback->setObjectName(QString::fromUtf8("labelback"));
        labelback->setGeometry(QRect(640, 0, 440, 640));
        labelback->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        labelid = new QLabel(centralWidget);
        labelid->setObjectName(QString::fromUtf8("labelid"));
        labelid->setGeometry(QRect(650, 60, 420, 30));
        labelid->setStyleSheet(QString::fromUtf8("font: 75 14pt \"Aharoni\";"));
        labellv = new QLabel(centralWidget);
        labellv->setObjectName(QString::fromUtf8("labellv"));
        labellv->setGeometry(QRect(650, 100, 420, 30));
        labellv->setStyleSheet(QString::fromUtf8("font: 75 14pt \"Aharoni\";"));
        labelhp = new QLabel(centralWidget);
        labelhp->setObjectName(QString::fromUtf8("labelhp"));
        labelhp->setGeometry(QRect(650, 150, 420, 30));
        labelhp->setStyleSheet(QString::fromUtf8("font: 75 14pt \"Aharoni\";"));
        labelatt = new QLabel(centralWidget);
        labelatt->setObjectName(QString::fromUtf8("labelatt"));
        labelatt->setGeometry(QRect(650, 180, 420, 30));
        labelatt->setStyleSheet(QString::fromUtf8("font: 75 14pt \"Aharoni\";"));
        labelsp = new QLabel(centralWidget);
        labelsp->setObjectName(QString::fromUtf8("labelsp"));
        labelsp->setGeometry(QRect(650, 220, 420, 30));
        labelsp->setStyleSheet(QString::fromUtf8("font: 75 14pt \"Aharoni\";"));
        labeltest = new QLabel(centralWidget);
        labeltest->setObjectName(QString::fromUtf8("labeltest"));
        labeltest->setGeometry(QRect(650, 260, 420, 30));
        labeltest->setStyleSheet(QString::fromUtf8("font: 75 14pt \"Aharoni\";"));
        labelout1 = new QLabel(centralWidget);
        labelout1->setObjectName(QString::fromUtf8("labelout1"));
        labelout1->setGeometry(QRect(650, 600, 420, 30));
        labelout1->setStyleSheet(QString::fromUtf8("font: 75 8pt \"Aharoni\";\n"
"text-decoration: underline;"));
        labelout3 = new QLabel(centralWidget);
        labelout3->setObjectName(QString::fromUtf8("labelout3"));
        labelout3->setGeometry(QRect(650, 540, 420, 30));
        labelout3->setStyleSheet(QString::fromUtf8("font: 8pt \"Simplified Arabic\";"));
        labelout5 = new QLabel(centralWidget);
        labelout5->setObjectName(QString::fromUtf8("labelout5"));
        labelout5->setGeometry(QRect(650, 480, 420, 30));
        labelout5->setStyleSheet(QString::fromUtf8("font: 8pt \"Simplified Arabic\";"));
        labelout4 = new QLabel(centralWidget);
        labelout4->setObjectName(QString::fromUtf8("labelout4"));
        labelout4->setGeometry(QRect(650, 510, 420, 30));
        labelout4->setStyleSheet(QString::fromUtf8("font: 8pt \"Simplified Arabic\";"));
        labelout2 = new QLabel(centralWidget);
        labelout2->setObjectName(QString::fromUtf8("labelout2"));
        labelout2->setGeometry(QRect(650, 570, 420, 30));
        labelout2->setStyleSheet(QString::fromUtf8("font: 8pt \"Simplified Arabic\";"));
        buttonstop = new QPushButton(centralWidget);
        buttonstop->setObjectName(QString::fromUtf8("buttonstop"));
        buttonstop->setGeometry(QRect(650, 310, 160, 40));
        buttonstop->setStyleSheet(QString::fromUtf8("font: 75 12pt \"Aharoni\";\n"
""));
        pushButton_2 = new QPushButton(centralWidget);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setGeometry(QRect(650, 410, 160, 40));
        pushButton_2->setStyleSheet(QString::fromUtf8("font: 75 12pt \"Aharoni\";"));
        buttonrelease = new QPushButton(centralWidget);
        buttonrelease->setObjectName(QString::fromUtf8("buttonrelease"));
        buttonrelease->setGeometry(QRect(650, 360, 160, 40));
        buttonrelease->setStyleSheet(QString::fromUtf8("font: 75 12pt \"Aharoni\";"));
        MainWindow->setCentralWidget(centralWidget);
        labelback->raise();
        labelturn->raise();
        labelid->raise();
        labellv->raise();
        labelhp->raise();
        labelatt->raise();
        labelsp->raise();
        labeltest->raise();
        labelout1->raise();
        labelout3->raise();
        labelout5->raise();
        labelout4->raise();
        labelout2->raise();
        buttonstop->raise();
        pushButton_2->raise();
        buttonrelease->raise();

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "FishGame", 0, QApplication::UnicodeUTF8));
        labelturn->setText(QString());
        buttonstop->setText(QApplication::translate("MainWindow", "stop", 0, QApplication::UnicodeUTF8));
        pushButton_2->setText(QApplication::translate("MainWindow", "result", 0, QApplication::UnicodeUTF8));
        buttonrelease->setText(QApplication::translate("MainWindow", "return", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
