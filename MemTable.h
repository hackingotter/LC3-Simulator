#ifndef MEMTABLE_H
#define MEMTABLE_H

#include <QWidget>
#include "QTableView"
#include "QAction"
#include "modeler.h"
#include "QPoint"
class MemTable : public QTableView
{
    Q_OBJECT
    void setupConnections();
    modeler * model;
    mem_loc_t * copied;
public:
    MemTable(modeler *model, QWidget *parent = 0);
public slots:
    void showClickOptions(const QPoint &pos);
    void scrollToRow(val_t row);
    void handleInsertRow();
    void handleCopy();
    void handlePasteOver();
    void handleCut();
    void scrollToSelected();
private slots:
};

#endif // MEMTABLE_H
