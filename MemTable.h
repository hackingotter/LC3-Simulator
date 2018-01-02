#ifndef MEMTABLE_H
#define MEMTABLE_H

#include <QWidget>
#include "QTableView"
#include "modeler.h"
class MemTable : public QTableView
{
    Q_OBJECT
public:
    MemTable(modeler *model, QWidget *parent = 0);
public slots:
    void showClickOptions(const QPoint &pos);
    void scrollToRow(val_t row);
};

#endif // MEMTABLE_H
