#ifndef WATCHWATCHER_H
#define WATCHWATCHER_H
#include <QWidget>
#include <computer.h>
#include <QMap>
#include <QPushButton>
#include <QTableView>
#include <qscrollarea.h>
#include <WatchModel.h>
class WatchWatcher:public QWidget
{
    Q_OBJECT
public:
    WatchWatcher(QWidget* parent = 0,bool* access = Q_NULLPTR);
    QTableView* getTableViewPtr();
    QTableView* Coat;
private:
    QPushButton* Delete;


    WatchModel* Levi;
    bool* _access;
};

#endif // WATCHWATCHER_H
