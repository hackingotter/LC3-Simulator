#include "WatchWatcher.h"
#include "QHBoxLayout"
#include "QVBoxLayout"
#include "computer.h"
#include "WatchModel.h"


WatchWatcher::WatchWatcher(QWidget *parent,bool* access): QWidget(parent),_access(access)
{
    qDebug("Initializing WatchWatcher");
//    Offset = new Q<val_t>();
    Coat = new QTableView(this);
    Delete = new QPushButton();

    QHBoxLayout* hlayout = new QHBoxLayout(this);
    hlayout->addWidget(Coat);
    hlayout->addWidget(Delete);
    setLayout(hlayout);
    Levi =new WatchModel(parent,_access);
    Coat->setModel(Levi);
    setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
}
QTableView * WatchWatcher::getTableViewPtr()
{
    return Coat;
}
