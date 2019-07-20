#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include <QCoreApplication>
int main(int argc, char *argv[])
{
    qSetMessagePattern("   Loc: [%{file}:%{line}] %{message}");//this provides an easy way to debug
    qDebug(QString().setNum(QCoreApplication::applicationPid()).toLocal8Bit());
    QCoreApplication::setApplicationName("LC-3 Sim");
    QCoreApplication::setApplicationVersion("0.1.0");
    QCoreApplication::setAttribute(Qt::AA_MacDontSwapCtrlAndMeta);
    QCoreApplication::setOrganizationName("Eastern Illinois University");
    QApplication a(argc, argv);//not gonna lie, don't know what this does
    MainWindow w;
    w.show();
    w.setWindowTitle(QCoreApplication::applicationName());
    w.importantInfo();

    return a.exec();
}
