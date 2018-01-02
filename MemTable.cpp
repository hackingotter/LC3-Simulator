#include "MemTable.h"
#include "QHeaderView"
#include "computer.h"
#include "Utility.h"
#define MEM_VIEW_BP_COL      0
#define MEM_VIEW_ADR_COL     1
#define MEM_VIEW_NAME_COL    2
#define MEM_VIEW_VAL_COL     3
#define MEM_VIEW_MNEM_COL    4
#define MEM_VIEW_COMMENT_COL 5




#define HEX_COLUMN_WIDTH 60
MemTable::MemTable(modeler* model,QWidget* parent):QTableView(parent)
{

    showGrid();
    setModel(model);
    verticalHeader()->hide();
    setColumnWidth(MEM_VIEW_BP_COL,30);
    horizontalHeader()->setSectionResizeMode(MEM_VIEW_BP_COL,QHeaderView::Fixed);

    setColumnWidth(MEM_VIEW_ADR_COL,HEX_COLUMN_WIDTH);
    horizontalHeader()->setSectionResizeMode(MEM_VIEW_ADR_COL,QHeaderView::Fixed);

    setColumnWidth(MEM_VIEW_NAME_COL,HEX_COLUMN_WIDTH);

    setColumnWidth(MEM_VIEW_VAL_COL,HEX_COLUMN_WIDTH);
    horizontalHeader()->setSectionResizeMode(MEM_VIEW_VAL_COL,QHeaderView::Fixed);

    setSelectionMode(QAbstractItemView::SingleSelection);
    setSelectionBehavior(QAbstractItemView::SelectRows);

    setContextMenuPolicy(Qt::CustomContextMenu);

    CONNECT(this,customContextMenuRequested(const QPoint &),
                this,showClickOptions(const QPoint &));

}
void MemTable::showClickOptions(const QPoint &pos)
{
    qDebug("clicking!");
    QMenu ClickMenu(tr("Some Text"),this);

    QAction test("Test action",this);

    ClickMenu.addAction(&test);
    ClickMenu.exec(mapToGlobal(pos  ));

}
void MemTable::scrollToRow(val_t row)
{
    qDebug("Scrolling to Row");
    scrollTo(model()->index(row,0),QAbstractItemView::PositionAtTop);
}
