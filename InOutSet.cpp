#include "InOutSet.h"
#include <QHBoxLayout>
#include <QVBoxLayout>

#include <QKeyEvent>
#include <QDebug>

#include "computer.h"



InOutSet::InOutSet(QWidget *parent) : QWidget(parent)
{

    setFocusPolicy(Qt::StrongFocus);


    QHBoxLayout* hLayout = new QHBoxLayout(this);
    QVBoxLayout* vLayout = new QVBoxLayout();

    Clear = new QPushButton(this);
    Clear->setText("Clear");

    Take = new QPushButton(this);
    Take->setText("Take");




    textDisplay = new QLabel(this);
    textDisplay->setStyleSheet("background-color:red;");
    textDisplay->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);


    setLayout(hLayout);
    hLayout->addWidget(textDisplay);
    hLayout->addLayout(vLayout);
    vLayout->addWidget(Clear);
    vLayout->addWidget(Take);
    textDisplay->setText("Hello");
}
void InOutSet::keyPressEvent(QKeyEvent *event)
{

    char c = event->text().at(0).toLatin1();
    qDebug("key was pressed");
    if(c>= 'a' && c<= 'g')
    {
        qDebug(c+ "");

        Computer::getDefault()->setKeyboardCharacter(c,true);
        event->accept();

    }


}
void InOutSet::update()
{

    QString n3w = QString(textDisplay->text()+ Computer::getDefault()->getKeyboardCharacter());
    qDebug(n3w.toLocal8Bit());
    textDisplay->setText(n3w);
}




