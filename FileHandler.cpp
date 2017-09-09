#include "FileHandler.h"
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include "Utility.h"
FileHandler::FileHandler(QObject *parent) : QObject(parent)
{
qDebug("hello");

}

QByteArray FileHandler::read()
{
    QByteArray data;
    QFile file("in.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return 0;

    QTextStream in(&file);
    // You could use readAll() here, too.
    while (!in.atEnd()) {
        QString line = in.readLine();
        data.append(line);
    }

    file.close();
    return data;
}

void FileHandler::write(QByteArray data)
{
    QFile file("out.txt");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

//    QTextStream out(&file);
    file.write(data);
    file.close();
}
