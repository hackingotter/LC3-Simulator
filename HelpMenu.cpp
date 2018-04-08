#include "HelpMenu.h"
#include "QWindow"
#include "QHBoxLayout"
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
    GeneralTab = generateGeneralTab();
    MemViewTab = generateMemViewTab();
    StackViewTab=generateStackViewTab();
    RegViewTab = generateRegViewTab();
    Tabs->addTab(GeneralTab,"General");
    Tabs->addTab(MemViewTab,"Memory");
    Tabs->addTab(StackViewTab,"Stack");
    Tabs->addTab(RegViewTab,"Registers");
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

    return stackTab;
}
QWidget* HelpMenu::generateRegViewTab()
{
    QWidget* RegViewTab = new QWidget();

    return RegViewTab;
}
