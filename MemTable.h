#ifndef MEMTABLE_H
#define MEMTABLE_H

#include <QWidget>
#include "QTableView"
#include "QAction"
#include "modeler.h"
#include "QPoint"

#define MINIMUMINSERTCOMBO (16)
class MemTable : public QTableView
{
    Q_OBJECT
    void setupConnections();
    bool flipped = false;
    modeler * model;
    mem_loc_t * copied;
    QModelIndex savedCurrentFocusIndex = QModelIndex();
    void selectedClickOptions(const QPoint &pos, QMenu *clickMenu);
    void saveSettings();
    /** put a row into the spot where target is, shifting the below
     * @brief insertRow
     * @param target
     */
    void insertRow(mem_addr_t target);
    /** setupWarnings
     * \todo DO
     */
    void setupWarnings();
public:
    MemTable(modeler *model, QWidget *parent = 0);
    void setupActions();
    void selectRange(mem_addr_t begin, int32_t end);
    /** Usually, when one hides or shows an object, the internal focus of that object shifts.
     * This results in seemingly random behavior, which is not what the user will expect.
     *
     * This function hides the MemTable, first recording the index which is in focus.
     */
    void hide();
    /** Usually, when one hides or shows an object, the internal focus of that object shifts.
     * This results in seemingly random behavior, which is not what the user will expect.
     *
     * This function shows the MemTable, but, after doing so, resets the focused index
     */
    void show();

    void setFlipped(bool upIsDown);
    void setButtonText(QString st);
    void requestKick();
public slots:
    void showClickOptions(const QPoint &pos);
    void scrollToRow(val_t row);
    void handleInsertRow();
    void handleCopy();
    void handlePasteOver();
    void scrollToSelected();
    void handleCut();
    void kick();
    /**
     * @brief goForward
     *
     * This is similar to the re/undo function, but instead of reverting the code to a future
     * state, it instead brings the view of the code to a future state.
     */
    void goForward();
    void goBackward();
    void changeDataDisplay(mem_addr_t row);
private slots:
    void setCut();
    void setCopied();
    void swap();
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
    void fastShiftDown(bool makeAgreement = true );
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
    void insertAbove();
    void insertBelow();
    void fastShiftUp(bool makeAgreement = true);

};

#endif // MEMTABLE_H
