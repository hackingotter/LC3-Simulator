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
    void selectedClickOptions(const QPoint &pos, QMenu *clickMenu);
public:
    MemTable(modeler *model, QWidget *parent = 0);
    void setupActions();
public slots:
    void showClickOptions(const QPoint &pos);
    void scrollToRow(val_t row);
    void handleInsertRow();
    void handleCopy();
    void handlePasteOver();
    void scrollToSelected();
    void handleShift();
    void handleCut();
private slots:
    void setCut();
    void setCopied();
    void swap();
    void shiftUp();
    void shiftDown();
};

#endif // MEMTABLE_H
