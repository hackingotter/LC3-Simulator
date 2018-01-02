#include "MemWindow.h"
#include "QVBoxLayout"
#include "QHBoxLayout"
#include "Utility.h"
MemWindow::MemWindow(modeler *model, QWidget *parent) : QWidget(parent)
{
    QHBoxLayout* hLayout = new QHBoxLayout();
    hLayout->setContentsMargins(0,0,0,0);
    Label = new QLabel("name");
    Label->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);


    Input = new QLineEdit(this);
    Input->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);


    GotoButton = new QPushButton("GOTO",this);
    GotoButton->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Fixed);
    GotoButton->setMinimumWidth(42);

    PCButton = new QPushButton("PC",this);
    PCButton->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Fixed);
    PCButton->setMaximumWidth(23);
    PCButton->setMaximumHeight(23);



    Label->setContentsMargins(0,500,0,0);
    hLayout->addWidget(Label);
    hLayout->addWidget(Input);
    hLayout->addWidget(GotoButton);
    hLayout->addWidget(PCButton);





    View = new MemTable(model,this);
    View->setContentsMargins(0,0,0,0);
    View->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::MinimumExpanding);

    QVBoxLayout* vLayout = new QVBoxLayout(this);
    vLayout->addLayout(hLayout);
    vLayout->setContentsMargins(0,0,0,0);
    vLayout->addWidget(View);
    setLayout(vLayout);



    /*
     *  Connections
     */
    //Since it feels natural to hit enter to enter the number
    connect(Input,SIGNAL(returnPressed()),GotoButton,SLOT(click()));


    //Making the Input Buttons Function
    connect(PCButton,SIGNAL(pressed()),this,SLOT(handlePCPress()));
    connect(GotoButton,SIGNAL(pressed()),this,SLOT(handleGotoPress()));

    //



}
void MemWindow::handlePCPress()
{
    View->scrollToRow(Computer::getDefault()->getRegister(PC));
}
void MemWindow::handleGotoPress()
{
    bool ok = true;
    int target = Utility::unifiedInput2Val(Input->text(),&ok);
    if(ok)
    {
    View->scrollToRow(target);
    }
    CLEAR(Input);

}

