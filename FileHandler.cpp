#include "FileHandler.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include "Utility.h"
FileHandler::FileHandler(QObject *parent) : QObject(parent)
{
qDebug("hello");

}
void FileHandler::trig()
{
    QFile* phil = new QFile("Text.txt");


    if(!phil->open(QIODevice::ReadOnly)) {
//        QMessageBox::information(0, "error", phil.errorString());
    }
    QTextStream reads(phil);
    while(!reads.atEnd())
    {
        qDebug(reads.readLine().toLocal8Bit());
    }
}
