#include <QApplication>
#include <QDesktopWidget>
#include "mywidget.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    mywidget w;
    w.show();

    return app.exec();
}

