#include "MemWindow.h"
#include "QVBoxLayout"
#include "QHBoxLayout"
#include "QString"
#include "Utility.h"
MemWindow::MemWindow(modeler *model, QWidget *parent) : QWidget(parent)
{
    QHBoxLayout* hLayout = new QHBoxLayout();
    hLayout->setContentsMargins(0,0,0,0);
    Label = new QLabel("name");
    Label->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);


    Input = new QLineEdit(this);
    Input->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    Input->setMaximumHeight(20);

    GotoButton = new QPushButton("GOTO",this);
    GotoButton->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Fixed);
    GotoButton->setMinimumWidth(42);
    GotoButton->setContentsMargins(0,0,0,0);

    /*
     * What I am about to initialize is a "Follow Button".
     *
     * The only difference between a Follow Button and a Push Button is that
     * the Follow Button sends out a signal whenever it is double clicked.
     */
    QString* temp = new QString("PC");
    SpecialButton = new FollowButton();
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
    connect(SpecialButton,SIGNAL(pressed()),this,SLOT(handlePCPress()));
    connect(GotoButton,SIGNAL(pressed()),this,SLOT(handleGotoPress()));
    connect(SpecialButton,SIGNAL(doubleClicked()), this,SLOT(handleTracking()));
    //



    SpecialButton->setStyleSheet(QString::fromUtf8("QPushButton:pressed"
                                                   "{ color: yellow }"
                                                   ));


}
void MemWindow::handlePCPress()
{
    bool ok = true;
    val_t target = getScrollOffset(Computer::getDefault()->getRegister(PC),&ok);
    if(ok)
    {

        View->scrollToRow(target);
    }
}
void MemWindow::handleGotoPress()
{
    bool ok = true;
    int target = Utility::unifiedInput2Val(Input->text(),&ok);

    if(ok)
    {
        int newtarget = getScrollOffset(target,&ok);
        if(ok)
        {
            mem_loc_t* bl = Computer::getDefault()->getMemLocationsBlock(0,5);
            Computer::getDefault()->setMemLocBlock(target,bl,5);

            View->scrollToRow(newtarget);
        }

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
        handlePCPress();
    }
}
int MemWindow::getScrollOffset(val_t row, bool* ok)
{
    *ok = true;
    qDebug(QString().setNum(row).toLocal8Bit());
    int upper = View->indexAt(View->rect().topLeft()).row();
    int lower = View->indexAt(View->rect().bottomLeft()).row();
    int offset = (lower - upper)/2;
    if(offset+row<0)
    {
        *ok=false;
        return 0;
    }
    if(row<offset)
        return row;
    return row-offset;
}


