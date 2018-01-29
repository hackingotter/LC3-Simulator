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
    this->model = model;
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

    setupConnections();


}
void MemTable::setupConnections()
{
    CONNECT(this,customContextMenuRequested(const QPoint &),
                this,showClickOptions(const QPoint &));
}
void MemTable::showClickOptions(const QPoint &pos)
{

    qDebug("clicking!");
    QMenu ClickMenu(tr("Some Text"),this);

    QAction test("Test action",this);

    QAction insert("Insert",this);
    QAction shift("Shift Down",this);

//    connect(insert,SIGNAL(QAction::triggered(bool)),this,SLOT(handleShift()));

    ClickMenu.addAction("Insert Row",this,SLOT(handleInsertRow()));
    ClickMenu.addAction("Copy",this,SLOT(handleCopy()));
    ClickMenu.addAction("Cut", this,SLOT(handleCut()));
    ClickMenu.addAction("Paste Over",this,SLOT(handlePasteOver()));
    ClickMenu.addAction(&shift);

//    connect(shift,SIGNAL(triggered()),this, handleShift());
    ClickMenu.exec(mapToGlobal(pos  ));

}
void MemTable::scrollToRow(val_t row)
{

    QString stylesheet = styleSheet();
    setStyleSheet("background-color:rgb(255,240,240)");

//    qDebug("Scrolling to Row");
    scrollTo(model->index(row,0),QAbstractItemView::PositionAtCenter);
    setStyleSheet(stylesheet);

}

void MemTable::handleInsertRow()
{
    qDebug("Hiya");
    QModelIndexList selected = this->selectedIndexes();

    int row = selected.at(0).row();
    Computer::getDefault()->Undos->beginMacro("Insert Row at " + getHexString(row));
    MASK
    for( int i = 1000; i >= 0;i--)
    {
        Computer::getDefault()->moveRow(row+i,row+i+1);
    }
    UNMASK
    Computer::getDefault()->Undos->endMacro();
}

void MemTable::handleCopy()
{
    //make a copy
    copied = (mem_loc_t*)calloc(1,sizeof(mem_loc_t));
    mem_loc_t temp = Computer::getDefault()->getMemLocation(this->selectedIndexes().at(0).row());
    copied->addr = temp.addr;
    copied->value = temp.value;

}
void MemTable::handleCut()
{

}
void MemTable::handlePasteOver()
{
    qDebug("Handling Paste");
    mem_addr_t addr = this->selectedIndexes().at(0).row();

    Computer::getDefault()->setMemLoc(addr,*copied);
}
