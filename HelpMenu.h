#ifndef HELPMENU_H
#define HELPMENU_H
#include <QTabWidget>
#include <QWidget>
#include <QDialog>

class HelpMenu : public QDialog
{
    Q_OBJECT
    QWidget* GeneralTab;
    QWidget* MemViewTab;
    QWidget* StackViewTab;
    QWidget* RegViewTab;
    QTabWidget* Tabs;
    QWidget *generateMemViewTab();
    QWidget *generateStackViewTab();
    QWidget *generateGeneralTab();
    QWidget *generateRegViewTab();
    void generateTabs();
public:
    explicit HelpMenu(QWidget *parent = nullptr);

signals:

public slots:
};

#endif // HELPMENU_H
