#include "MemWindow.h"
#include "QVBoxLayout"
#include "QHBoxLayout"
#include "QString"
#include "Utility.h"
MemWindow::MemWindow(modeler *model, HighlightScrollBar* scroll, QWidget *parent,QString* buttonName) : QWidget(parent)
{
    QHBoxLayout* hLayout = new QHBoxLayout();
    hLayout->setContentsMargins(0,0,0,0);
    Label = new QLabel("Memory View");
    Label->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);


    Input = new QLineEdit(this);
    Input->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    Input->setMaximumHeight(20);

    GotoButton = new QPushButton("GOTO",this);
    GotoButton->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);
    GotoButton->setMinimumWidth(40);
    GotoButton->setContentsMargins(0,0,0,0);

    /*
     * What I am about to initialize is a "Follow Button".
     *
     * The only difference between a Follow Button and a Push Button is that
     * the Follow Button sends out a signal whenever it is double clicked.
     */

    SpecialButton = new FollowButton(*buttonName);
    SpecialButton->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Fixed);
    SpecialButton->setMaximumWidth(23);
    SpecialButton->setMaximumHeight(23);



    Label->setContentsMargins(0,0,0,0);
    hLayout->addWidget(Label);
    hLayout->addWidget(Input);
    hLayout->addWidget(GotoButton);
    hLayout->addWidget(SpecialButton);





    View = new MemTable(model,this);
    View->setContentsMargins(0,0,0,0);
    View->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Expanding);
    View->setMinimumHeight(10);
    QVBoxLayout* vLayout = new QVBoxLayout(this);
    vLayout->addLayout(hLayout);
    vLayout->setContentsMargins(0,0,0,0);
    vLayout->addWidget(View);
    setLayout(vLayout);
    vLayout->setSpacing(0);
    hLayout->setSpacing(0);

    setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
    /*
     *  Connections
     */
    //Since it feels natural to hit enter to enter the number
    connect(Input,SIGNAL(returnPressed()),GotoButton,SLOT(click()));


    //Making the Input Buttons Function
    connect(SpecialButton,SIGNAL(pressed()),this,SLOT(handleSRPress()));
    connect(GotoButton,SIGNAL(pressed()),this,SLOT(handleGotoPress()));
    connect(SpecialButton,SIGNAL(doubleClicked()), this,SLOT(handleTracking()));
    //



    SpecialButton->setStyleSheet(QString::fromUtf8("QPushButton:pressed"
                                                   "{ color: yellow }"
                                                   ));


}

MemTable *MemWindow::getMemView()
{
    return View;
}
void MemWindow::handleSRPress()
{
    View->scrollToRow(Computer::getDefault()->getRegister(SpecialReg));
}
void MemWindow::handleGotoPress()
{
    bool ok = true;
    int target = Utility::unifiedInput2Val(Input->text(),&ok);

    if(ok)
    {

        for(int i = 5;i>=0;i--)
        {
            Computer::getDefault()->setMemLoc(target+i,Computer::getDefault()->getMemLocation(i));
        }

        View->scrollToRow(target);
    }
    CLEAR(Input);

}
void MemWindow::handleTracking()
{
    qDebug("double");

    followmode=!followmode;
    SpecialButton->setStyleSheet(QString("QPushButton:enabled"
                                         "{ Background-color: ")+QString((followmode)?"yellow":"light grey")+QString("}")
                                 );
    //    PCButton->setStyleSheet("{background-color:rgb(150,150,255)}");
}

void MemWindow::setSpecialReg(reg_t reg)
{
    SpecialReg = reg;
}
void MemWindow::kick()
{
    View->hide();
    View->show();
    if(followmode)
    {
        handleSRPress();
    }
}


