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
    TABUTILITY(QuickStartTab,generateQuickStartTab(),"Quick Start",Tabs);
    TABUTILITY(GeneralTab   ,generateGeneralTab(),"General",    Tabs);
    TABUTILITY(MemViewTab   ,generateMemViewTab(),"Memory and Stack",     Tabs);
//    TABUTILITY(StackViewTab ,generateStackViewTab(),"Stack",    Tabs);
    TABUTILITY(RegViewTab   ,generateRegViewTab(),"Registers",  Tabs);
//    TABUTILITY(SettingTab   ,generateSettingTab(),"Settings",   Tabs);
//    TABUTILITY(ContactTab   ,generateContactTab(),"Contact",    Tabs);
    SettingTab->deleteLater();
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
                   "Right now this program is in Beta.  There are many bugs, many things that aren't implemented,"
                   " and somethings are implemented incorrectly, or worse partially correctly.\n"
                   "\n"
                   "In light of this, I beg that you heed this warning:\n"
                   "\n"
                   "Do not trust this simulator!\n"
                   "\n"
                   "I can guarentee that the assembler won't break your file, but I can't guarentee that the assembler "
                   "is fully functional.  If you must save, it can be done by typing \n\n\t'save x<starting_address> x<ending_address>'\n\n"
                   "into the console, and it will try to save the file.\n"
                   "I can't guarentee that the save will work.  It probably will, but I beg that you make your adjustments either"
                   "with great care or somewhere that has reputable saving features, like notepad.exe or any word processor.\n"
                   "\n"
                   "Known bad implementations:\n\n"
                   "\t The console doesn't really do much, and what it does do it doesn't clearly communicate that.  Just don't use it unless"
                   "you feel like risking it all and saving.\n\n"
                   "\t Don't undo with input.  It will likely crash"
                   "\n"
                   "\n"

                   "Bugs:\n"
                   "\n"
                   "\t There are going to be bugs.  I apologize beforehand for any time lost, but know that every bug you find for"
                   "me is a bug someone won't suffer in a future version.\n"
                   "\n"
                   "\t If you are able to find a bug, I ask that you first check that you have the newest version of the simulator."
                   "you can reach me at jbmelberg@eiu.edu\n"
                   "Please give me a description of what you were doing, what happened, and the code that you were using. I can't promise "
                   "that I will be able to fix it that day, but I plan on bugfixing on a weekly basis.\n"
                   "\n"
                   "If you would like to avoid the bugs, just follow these tips:\n\n"
                   "\tUse a different simulator.  This program is in Beta, so there are bugs that you can see and bugs you can't see.\n\n"
                   "\tUse the interface slowly, as not every button is shut off when it should be.\n\n"
                   "\tCheck the performance against a different simulator.\n"
                   "\n"
                   "Suggestions:\n"
                   "\n"
                   "\n If you think of a feature which you would like to see added, please let me know.  I have much planned for "
                   "this simulator, and your input is definitely appreciated.\n"
                   "\n"
                   "\n"
                   "Things to do and see:\n"
                   "\n"
                   "\t Down in the bottom right corner, there is a box that says \"<empty>\".  This is one of the most useful features I "
                   "have ever implemented.\n"
                   "That thing lets you run and unrun code!  It is sorta buggy, like the rest of the program, but it is something that "
                   "I sorely miss whenever I code some other language.  Treat it gently, and it will likely serve you well."
                   "");


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
    QTabWidget* stackInfo = new QTabWidget();
    QHBoxLayout* QHB = new QHBoxLayout();
    stackTab->setLayout(QHB);
    stackTab->layout()->addWidget(stackInfo);
    QWidget* TipsNTricks = new QWidget();
    TipsNTricks->setLayout(new QHBoxLayout());
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

    stackInfo->addTab(TipsNTricks,"Tips && Tricks");
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
//    QScrollArea* QSA = new QScrollArea(genTab);
//    QSA->setWidget(generalInfo);
//    QSA->setSizePolicy(QUICKER,QUICKER);


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
