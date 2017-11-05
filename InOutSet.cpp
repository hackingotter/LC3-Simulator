#include "InOutSet.h"
#include <QHBoxLayout>
#include <QVBoxLayout>

#include <QKeyEvent>
#include <QDebug>

#include "computer.h"



InOutSet::InOutSet(QWidget *parent) : QWidget(parent)
{

    qDebug("Initializing InOutSet");
    setFocusPolicy(Qt::StrongFocus);


    QHBoxLayout* hLayout = new QHBoxLayout(this);
    QVBoxLayout* vLayout = new QVBoxLayout();

    Clear = new QPushButton(this);
    Clear->setText("Clear");

    Take = new QPushButton(this);
    Take->setText("Take");

    CONNECT(Take,pressed(),this,update());


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
    qDebug("key pressed");
    char c = event->text().at(0).toLatin1();

    if(c>= 'a' && c<= 'g')
    {
        qDebug("A");

        Computer::getDefault()->setKeyboardCharacter('A',true);
//        update();
        event->accept();

    }


}
void InOutSet::update()
{

    QString n3w = QString(textDisplay->text()+ QString((Computer::getDefault()->getKeyboardCharacter())));
    qDebug(n3w.toLocal8Bit());
    textDisplay->setText(n3w);
}




