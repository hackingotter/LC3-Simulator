#include "InOutSet.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QChar>
#include <QKeyEvent>
#include <QDebug>

#include <QLabel>
#include "computer.h"
#include <QUndoCommand>



InOutSet::InOutSet(QWidget *parent) : QWidget(parent)
{

    qDebug("Initializing InOutSet");



    QHBoxLayout* hLayout = new QHBoxLayout(this);
    QVBoxLayout* vLayout = new QVBoxLayout();

    InputMonitor = new QLabel("");
    InputMonitor->setText("I");

    OutputMonitor = new QLabel();
    OutputMonitor->setText("O");

    connect(OutputMonitor,SIGNAL(released()),this,SLOT(kick()));
    connect(Computer::getDefault(),SIGNAL(pushDisplay(val_t)),this,SLOT(pushChar(val_t)));
    connect(Computer::getDefault(),SIGNAL(popDisplay()),this,SLOT(popChar()));
//    CONNECT(Take,QPushButton::pressed(),this,update());




    textDisplay = new QLabel(this);
    textDisplay->setStyleSheet("background-color:x0000FFF;");
    textDisplay->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    textDisplay->setWordWrap(true);
    textDisplay->setAlignment(Qt::AlignTop|Qt::AlignLeft);
    textDisplay->setFocusPolicy(Qt::StrongFocus);
    textDisplay->setStyleSheet(":focus{background-color:rgb(150,150,255)}");


    setLayout(hLayout);
    hLayout->addWidget(textDisplay);
    hLayout->addLayout(vLayout);
    vLayout->addWidget(InputMonitor);
    vLayout->addWidget(OutputMonitor);
    textDisplay->setText("");
//    this->setStyleSheet("background-color:rgb(150,150,150)");

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
    textDisplay->setText("");
}
void InOutSet::update()
{


}
void InOutSet::popChar()
{
    QString text = textDisplay->text();
    if(text.length()>=1)
    {
        text.chop(1);
    }
    textDisplay->setText(text);
}
void InOutSet::pushChar(val_t val)
{

//    Computer::getDefault()->setMemValue(DSR,0x0000);

    qDebug("pushChar requested");
    qDebug(QString().setNum(val).toLocal8Bit());
    textDisplay->setText(textDisplay->text()+(char)val);
//    Computer::getDefault()->setMemValue(DSR,Computer::getDefault()->getMemValue(DSR)&0x8000);
}

void InOutSet::kick()
{
    qDebug("Kicked");
//    Computer::getDefault()->setMemValue(DDR,Computer::getDefault()->getMemValue(KBDR));
}


