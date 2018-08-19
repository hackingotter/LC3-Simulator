#include "MemWindow.h"
#include "QVBoxLayout"
#include "QHBoxLayout"
#include "QString"
#include "QCompleter"
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

    setupInput();
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
void MemWindow::setupInput()
{

    labelDict = std::map<QString,uint16_t>();
    QStringList suggestions;
#define SUGGEST(KEY,VALUE)\
    suggestions<<(KEY);\
    labelDict[KEY] = (VALUE);

    for(val_t i = 0;i!=MEMSIZE;i++)
    {
        label_t* lab = Computer::getDefault()->getMemLabel(i);
        if(lab)
        {
                SUGGEST(lab->name,i)

        }
    }
    for(int i =0;i<8/*number of general purpose registers*/;i++)
    {
        const QString reg ="r"+QString().setNum(i);
        SUGGEST(reg,Computer::getDefault()->getRegister((reg_t)i))
//        suggestions<<reg;
//        labelDict[reg]= Computer::getDefault()->getRegister((reg_t)i);
    }
    SUGGEST("PC",Computer::getDefault()->getRegister(PC))
    SUGGEST("SP",Computer::getDefault()->getRegister(SPR))
    QCompleter searchCompleter(suggestions,this);

    searchCompleter.setCaseSensitivity(Qt::CaseInsensitive);
    searchCompleter.setCompletionMode(QCompleter::UnfilteredPopupCompletion);
    Input->setCompleter(&searchCompleter);
//    connect(searchCompleter,&QCompleter::activated)
}

MemTable *MemWindow::getMemView()
{
    return View;
}

void MemWindow::setFlipped(bool upIsDown)
{
    View->setFlipped(upIsDown);
    if(upIsDown)
    {
        SpecialReg = SPR;
        SpecialButton->setText("SP");

        QString red = QString().setNum(R6COLOR.red());
        QString green = QString().setNum(R6COLOR.green());
        QString blue    = QString().setNum(R6COLOR.blue());
        SpecialButton->setStyleSheet("QPushButton:pressed{ color:  rgb(" +red + "," + green + "," + blue +")}");
    }
    else
    {
        SpecialReg = PC;
        SpecialButton->setText("PC");
        SpecialButton->setStyleSheet(QString::fromUtf8("QPushButton:pressed"
                                                       "{ color: yellow }"
                                                       ));
    }
}
void MemWindow::handleSRPress()
{
    val_t target_address = Computer::getDefault()->getRegister(SpecialReg);
    View->scrollToRow(target_address);
}
void MemWindow::handleGotoPress()
{
    bool ok = true;
    int target = Utility::unifiedInput2Val(Input->text(),&ok);

    if(ok)
    {
        View->scrollToRow(target);
    }
    else
    {
        const QString text = Input->text();
        if(labelDict.find(text)!=labelDict.end())
        {
            View->scrollToRow(labelDict[text]);
        }
    }
    CLEAR(Input);

}
void MemWindow::handleTracking()
{
    qDebug("double");

    followmode=!followmode;


    QString red = QString().setNum(R6COLOR.red());
    QString green = QString().setNum(R6COLOR.green());
    QString blue    = QString().setNum(R6COLOR.blue());
    QString r6colorString = "rgb(" +red + "," + green + "," + blue +")}";
    QString color;
    switch(SpecialReg)
    {
    case PC:
        color = "yellow";
        break;
    case R6:
        color = r6colorString;
        break;
    }
    SpecialButton->setStyleSheet(QString("QPushButton:enabled"
                                         "{ Background-color: ")+QString((followmode)?color:"light grey")+QString("}")
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
    setupInput();
}


