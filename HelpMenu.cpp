#include "HelpMenu.h"
#include "QWindow"
#include "QLabel"
#include "QHBoxLayout"
#include "QTabWidget"
#include "QCoreApplication"
#include "QTextEdit"
#include "QGridLayout"

#include "QScrollArea"

#define TABUTILITY(SOURCE,GENERATOR,NAME,TARGET)\
SOURCE = GENERATOR;\
    TARGET->addTab(SOURCE,NAME);
HelpMenu::HelpMenu(QWidget *parent) : QDialog(parent)
{
    generateTabs();

    QHBoxLayout* QHB = new QHBoxLayout();
    QHB->addWidget(Tabs);
    this->setLayout(QHB);

    setWindowTitle("Info");
    setBaseSize(400,400);
    resize(700,600);


}

void HelpMenu::generateTabs()
{
    Tabs = new QTabWidget(this);
//    TABUTILITY(QuickStartTab,generateQuickStartTab(),"Quick Start",Tabs);
    TABUTILITY(GeneralTab   ,generateGeneralTab(),"General",    Tabs);
//    TABUTILITY(MemViewTab   ,generateMemViewTab(),"Memory and Stack",     Tabs);
//    TABUTILITY(StackViewTab ,generateStackViewTab(),"Stack",    Tabs);
//    TABUTILITY(RegViewTab   ,generateRegViewTab(),"Registers",  Tabs);
//    TABUTILITY(SettingTab   ,generateSettingTab(),"Settings",   Tabs);
//    TABUTILITY(ContactTab   ,generateContactTab(),"Contact",    Tabs);
//    SettingTab->deleteLater();
}
QWidget* HelpMenu::generateGeneralTab()
{
    QWidget* genTab = new QWidget();
    QTextEdit* generalInfo = new QTextEdit("A");
    generalInfo->setParent(genTab);
    QGridLayout* QVB = new QGridLayout(genTab);
    genTab->setLayout(QVB);

    QString* info = new QString("Hello!\n"
                   "\n"
                   "Welcome to LC3-Sim v"+QCoreApplication::applicationVersion()+
                   "\n"
                   "\n"
                   "Please read the README.txt and Appendix for more information.");


    generalInfo->setText(*info);
    generalInfo->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    generalInfo->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
#define QUICKER QSizePolicy::MinimumExpanding
    generalInfo->setSizePolicy(QUICKER,QUICKER);
//    QScrollArea* QSA = new QScrollArea(genTab);
//    QSA->setWidget(generalInfo);
//    QSA->setSizePolicy(QUICKER,QUICKER);


    QVB->addWidget(generalInfo,1,0);
    generalInfo->adjustSize();

    genTab->setSizePolicy(QUICKER,QUICKER);
    return genTab;
}
QWidget* HelpMenu::generateMemViewTab()
{
    QWidget* stackTab = new QWidget();
//    QTabWidget* stackInfo = new QTabWidget();
//    QHBoxLayout* QHB = new QHBoxLayout();
//    stackTab->setLayout(QHB);
//    stackTab->layout()->addWidget(stackInfo);
//    QWidget* TipsNTricks = new QWidget();
//    TipsNTricks->setLayout(new QHBoxLayout());
//    QLabel * tips = new QLabel(
//     "Here is where the memory of the machine is displayed.  For more info about the system memory, please address the included Appendix."
//                "\n"
//     "\n"
//     "What to know:"
//     "  BR stands for breakpoint, and can be set to halt execution.  If a register points to that line, this box will be filled with that register's color.\n"
//     "  Addr is the hex address of the line in memory.\n"
//     "  Name is the label of that line.\n"
//     "  Value is the value of a given line, and will dictate what Mnem displays.\n"
//     "  Mnem is the mnemonic (if it exists) for that line's value.\n"
//     "  Comments are any comments that exist for that line.\n"
//     "\n"
//     "  "


//                );

//    TipsNTricks->layout()->addWidget(tips);

//    stackInfo->addTab(TipsNTricks,"Tips && Tricks");
    return stackTab;
//    return memTab;
}
QWidget* HelpMenu::generateStackViewTab()
{
    QWidget* stackTab = new QWidget();
    QTabWidget* stackInfo = new QTabWidget();
    QHBoxLayout* QHB = new QHBoxLayout();
    stackTab->setLayout(QHB);
    stackTab->layout()->addWidget(stackInfo);
    QWidget* TipsNTricks = new QWidget();
    TipsNTricks->setLayout(new QHBoxLayout());
    QLabel * tips = new QLabel("You might have noticed that the Stack View has some weird discoloration.\n This is because, whenever a value is changed in that range,"
                               "a color is generated based on the value of ");

    TipsNTricks->layout()->addWidget(tips);

    stackInfo->addTab(TipsNTricks,"Tips && Tricks");
    return stackTab;
}
QWidget* HelpMenu::generateRegViewTab()
{
    QWidget* RegViewTab = new QWidget();

    return RegViewTab;
}

QWidget *HelpMenu::generateQuickStartTab()
{
    QWidget* quickStartTab = new QWidget();
    QTextEdit* quickstartTE = new QTextEdit("A");
    quickstartTE->setParent(quickStartTab);
    QGridLayout* QVB = new QGridLayout(quickStartTab);
    quickStartTab->setLayout(QVB);

    QString* info =
            new QString(
            "To Load, go to the menu bar,  press assemble and load.\n\n"
            "To Save, go to the Console Tab, type ``save x{begining line number} x{end line number}``"
            ", replacing the values between the curly brackets with the hexadecimal line numbers that inclusively contain the program."
                        );
    quickstartTE->setText(*info);
    quickstartTE->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    quickstartTE->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    quickstartTE->setSizePolicy(QUICKER,QUICKER);
    QScrollArea* QSA = new QScrollArea(quickStartTab);
    QSA->setWidget(quickStartTab);
    QSA->setSizePolicy(QUICKER,QUICKER);


    QVB->addWidget(quickstartTE,1,0);
    quickstartTE->adjustSize();

    quickStartTab->setSizePolicy(QUICKER,QUICKER);
    return quickStartTab;
}
QWidget *HelpMenu::generateContactTab()
{
    QWidget* stackTab = new QWidget();
    QTabWidget* stackInfo = new QTabWidget();
    QHBoxLayout* QHB = new QHBoxLayout();
    stackTab->setLayout(QHB);
    stackTab->layout()->addWidget(stackInfo);
    QWidget* TipsNTricks = new QWidget();
    TipsNTricks->setLayout(new QHBoxLayout());
    QLabel * tips = new QLabel("You might have noticed that the Stack View has some weird discoloration.\n This is because, whenever a value is changed in that range,"
                               "a color is generated based on the value of ");

    TipsNTricks->layout()->addWidget(tips);

    stackInfo->addTab(TipsNTricks,"Tips && Tricks");
    return stackTab;
}
QWidget *HelpMenu::generateSettingTab()
{
    QWidget* setTab = new QWidget();
    QTabWidget* holder = new QTabWidget(setTab);
    QWidget* shortcuts;
//    TABUTILITY(shortcuts,new QWidget(holder),"Shortcuts",holder);




    return setTab;
}
