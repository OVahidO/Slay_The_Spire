#include "mainwindow.h"
#include "gameplay.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //MainWindow w;
    //w.show();
    GamePlay g;
    g.show();
    return QApplication::exec();
}
