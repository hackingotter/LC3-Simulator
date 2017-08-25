#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
int main(int argc, char *argv[])
{
    qSetMessagePattern("   Loc: [%{file}:%{line}] %{message}");//this provides an easy way to debug
    QApplication a(argc, argv);//not gonna lie, don't know what this does
    MainWindow w;
    w.show();
    return a.exec();
}
