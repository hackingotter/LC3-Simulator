#ifndef HELPMENU_H
#define HELPMENU_H
#include <QTabWidget>
#include <QWidget>
#include <QDialog>

class HelpMenu : public QDialog
{
    Q_OBJECT
    QWidget* GeneralTab;
    QWidget* QuickStartTab;
    QWidget* MemViewTab;
    QWidget* StackViewTab;
    QWidget* RegViewTab;
    QWidget* SettingTab;
    QWidget* ContactTab;
    QTabWidget* Tabs;
    QWidget *generateQuickStartTab();
    QWidget *generateMemViewTab();
    QWidget *generateStackViewTab();
    QWidget *generateGeneralTab();
    QWidget *generateRegViewTab();
    QWidget *generateSettingTab();
    QWidget *generateContactTab();
    void generateTabs();
public:
    explicit HelpMenu(QWidget *parent = nullptr);

signals:

public slots:
};

#endif // HELPMENU_H
