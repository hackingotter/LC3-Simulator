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
    QModelIndex savedCurrentFocusIndex;
    modeler * model;
    mem_loc_t * copied;
    void selectedClickOptions(const QPoint &pos, QMenu *clickMenu);
    void saveSettings();
public:
    MemTable(modeler *model, QWidget *parent = 0);
    void setupActions();
    void selectRange(mem_addr_t begin, int32_t end);
    void hide();
    void show();
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
    /** Takes the selected indexes and shifts them up by one.
     * \param makeAgreement decides if the code is modified to keep pointers
     * intact.
     */
    void shiftUp(bool makeAgreement = true);
    /** Takes the selected indexes and shifts them up by one.
     * \param makeAgreement decides if the code is modified to keep pointers
     * intact.
     */
    void shiftDown(bool makeAgreement = true);
    /** For when possible connections in data are to be disregarded when moved,
     * this function calls for the data to be shifted down 'as is.'
     */
    void shiftDownBrute();
    /** For when possible connections in data are to be disregarded when moved,
     * this function calls for the data to be shifted up 'as is.'
     */
    void shiftUpBrute();
    void paste(bool makeAgreement = true);
    void pasteBrute();

};

#endif // MEMTABLE_H
