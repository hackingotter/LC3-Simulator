#include "InOutSet.h"
#include <QKeyEvent>
#include <QColor>
#include <QLabel>
#include <QDebug>
InOutSet::OutputBoxSet(QWidget * parent)
{

}

void InOutSet::keyPressEvent(QKeyEvent *ev)
{

    Computer::getDefault()->setKeyboardCharacter(ev->text().toLocal8Bit().at(0),true);
    qDebug(ev->text().toLocal8Bit());
    setStyleSheet("background-color: yellow");

}
void InOutSet::update()
{

    setText(text()+Computer::getDefault()->getKeyboardCharacter());
    qDebug(text().toLocal8Bit());
    setStyleSheet("background-color: white");
}
