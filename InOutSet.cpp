#include "InOutSet.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QChar>
#include <QKeyEvent>
#include <QDebug>

#include <QLabel>
#include "computer.h"
#include <QUndoCommand>


namespace Action {

};


InOutSet::InOutSet(QWidget *parent) : QWidget(parent)
{

    qDebug("Initializing InOutSet");



    QHBoxLayout* hLayout = new QHBoxLayout(this);
    QVBoxLayout* vLayout = new QVBoxLayout();

    Clear = new QPushButton(this);
    Clear->setText("Clear");

    Take = new QPushButton(this);
    Take->setText("Take");

    connect(Take,SIGNAL(released()),this,SLOT(kick()));
    connect(Computer::getDefault(),SIGNAL(pushDisplay(val_t)),this,SLOT(pushChar(val_t)));
    connect(Computer::getDefault(),SIGNAL(popDisplay()),this,SLOT(popChar()));
//    CONNECT(Take,QPushButton::pressed(),this,update());


    textDisplay = new QLabel(this);
    textDisplay->setStyleSheet("background-color:x0000FFF;");
    textDisplay->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    textDisplay->setWordWrap(true);
    textDisplay->setAlignment(Qt::AlignTop|Qt::AlignLeft);
    textDisplay->setFocusPolicy(Qt::StrongFocus);
    textDisplay->setStyleSheet(":focus{background-color:blue}");
    setLayout(hLayout);
    hLayout->addWidget(textDisplay);
    hLayout->addLayout(vLayout);
    vLayout->addWidget(Clear);
    vLayout->addWidget(Take);
    textDisplay->setText("Hello");

}
void InOutSet::keyPressEvent(QKeyEvent *event)
{
    qDebug("key pressed processing begin");
    event->accept();

    qDebug(QString().setNum(event->key()).toLocal8Bit());
    char temp = '\0';
    char* input = &temp;


        switch(event->key())
        {
#ifndef QT_NO_DEBUG_OUTPUT
        case 0x01000037:/*F8, for easy intake*/
            update();
            return;

#endif

        case 0x01000004://return
        case 0x01000005:{qDebug("Enter");*input = ('\n');break;}
        default:
            {
            if(event->key()<128)
                {
                    qDebug("Normal");
                    *input = event->key();
                }
            }
        }
        if(input != nullptr) Computer::getDefault()->setKeyboardCharacter(*input,true);


    qDebug("done with key");

}
void InOutSet::clearText()
{


}
void InOutSet::update()
{


}
void InOutSet::popChar()
{
    QString text = textDisplay->text();
    text.chop(1);
    textDisplay->setText(text);
}
void InOutSet::pushChar(val_t val)
{
    qDebug("pushChar requested");
    textDisplay->setText(textDisplay->text()+(char)val);
}

void InOutSet::kick()
{
    qDebug("Kicked");
    Computer::getDefault()->setMemValue(DDR,Computer::getDefault()->getMemValue(KBDR));
}


