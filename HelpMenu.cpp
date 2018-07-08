#include "HelpMenu.h"
#include "QWindow"
#include "QLabel"
#include "QHBoxLayout"
#include "QTabWidget"


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
    TABUTILITY(GeneralTab  ,generateGeneralTab(),"General",Tabs);
    TABUTILITY(MemViewTab  ,generateMemViewTab(),"Memory",Tabs);
    TABUTILITY(StackViewTab,generateStackViewTab(),"Stack",Tabs);
    TABUTILITY(RegViewTab  ,generateRegViewTab(),"Registers",Tabs);
    TABUTILITY(SettingTab  ,generateSettingTab(),"Settings", Tabs);
}
QWidget* HelpMenu::generateGeneralTab()
{
    QWidget* genTab = new QWidget();



    return genTab;
}
QWidget* HelpMenu::generateMemViewTab()
{
    QWidget* memTab = new QWidget();



    return memTab;
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

QWidget *HelpMenu::generateSettingTab()
{
    QWidget* setTab = new QWidget();
    QTabWidget* holder = new QTabWidget(setTab);
    QWidget* shortcuts;
    TABUTILITY(shortcuts,new QWidget(holder),"Shortcuts",holder);




    return setTab;
}
