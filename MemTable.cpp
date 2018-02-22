#include "MemTable.h"
#include "QHeaderView"
#include "computer.h"
#include "iostream"
#include "Utility.h"
#include "QShortcut"
#include "shortcutslisting.h"
#define MEM_VIEW_BP_COL      0
#define MEM_VIEW_ADR_COL     1
#define MEM_VIEW_NAME_COL    2
#define MEM_VIEW_VAL_COL     3
#define MEM_VIEW_MNEM_COL    4
#define MEM_VIEW_COMMENT_COL 5




#define HEX_COLUMN_WIDTH 60

#define ADDSHORTCUT(NAME,KEY,FUNCTION)\
{\
    QShortcut* cut = new QShortcut(this);\
    cut->setKey(KEY);\
    connect(cut,SIGNAL(activated()),this,SLOT(FUNCTION));\
    cut->setContext(Qt::WidgetShortcut);\
    cut->setObjectName(NAME);\
    }
MemTable::MemTable(modeler* model,QWidget* parent):QTableView(parent)
{

    showGrid();
    setModel(model);
    this->model = model;
    connect(this->model, SIGNAL(change()),this, SLOT(kick()));
    verticalHeader()->hide();
    setColumnWidth(MEM_VIEW_BP_COL,30);
    horizontalHeader()->setSectionResizeMode(MEM_VIEW_BP_COL,QHeaderView::Fixed);

    setColumnWidth(MEM_VIEW_ADR_COL,HEX_COLUMN_WIDTH);
    horizontalHeader()->setSectionResizeMode(MEM_VIEW_ADR_COL,QHeaderView::Fixed);

    setColumnWidth(MEM_VIEW_NAME_COL,HEX_COLUMN_WIDTH);

    setColumnWidth(MEM_VIEW_VAL_COL,HEX_COLUMN_WIDTH);
    horizontalHeader()->setSectionResizeMode(MEM_VIEW_VAL_COL,QHeaderView::Fixed);

    horizontalHeader()->setSectionResizeMode(MEM_VIEW_COMMENT_COL,QHeaderView::Stretch);
    //    setSelectionMode(QAbstractItemView::SingleSelection);
    setSelectionBehavior(QAbstractItemView::SelectRows);

    setContextMenuPolicy(Qt::CustomContextMenu);

    setupConnections();

    setupActions();
}


void MemTable::setupActions()
{
    qDebug("JJ");
    QShortcut* copy = new QShortcut(this);
    copy->setKey(Qt::CTRL + Qt::Key_C);
    connect(copy,SIGNAL(activated()),this,SLOT(setCopied()));
    copy->setContext(Qt::WidgetShortcut);

    QShortcut* cut = new QShortcut(this);
    cut->setKey(Qt::CTRL + Qt::Key_X);
    connect(cut,SIGNAL(activated()),this,SLOT(setCut()));
    cut->setContext(Qt::WidgetShortcut);

    QShortcut* budgeUp = new QShortcut(this);
    budgeUp->setKey(Qt::CTRL+Qt::SHIFT+Qt::Key_Up);
    connect(budgeUp,SIGNAL(activated()),this, SLOT(shiftUp()));
    budgeUp->setContext(Qt::WidgetShortcut);

    QShortcut* budgeDown= new QShortcut(this);
    budgeDown->setKey(Qt::CTRL+Qt::SHIFT+Qt::Key_Down);
    connect(budgeDown,SIGNAL(activated()),this, SLOT(shiftDown()));
    budgeDown->setContext(Qt::WidgetShortcut);
    ADDSHORTCUT("Paste"     ,Qt::CTRL + Qt::Key_V                          ,paste());
    ADDSHORTCUT("Paste Above",Qt::CTRL+ Qt::SHIFT + Qt::Key_V              ,pasteAbove());
    ADDSHORTCUT("Force Down",Qt::CTRL + Qt::SHIFT + Qt::ALT + Qt::Key_Down ,shiftDownBrute());
    ADDSHORTCUT("Force Up"  ,Qt::CTRL + Qt::SHIFT + Qt::ALT + Qt::Key_Up   ,shiftUpBrute());

}
void MemTable::setupConnections()
{
    CONNECT(this,customContextMenuRequested(const QPoint &),
            this,showClickOptions(const QPoint &));
}
void MemTable::showClickOptions(const QPoint &pos)
{
    mem_addr_t row = rowAt(pos.y());



    qDebug("clicking!");
    QMenu ClickMenu(tr("Some Text"),this);

    QAction test("Test action",this);

    QAction insert("Insert",this);
    QAction shift("Shift Down",this);


    //    connect(insert,SIGNAL(QAction::triggered(bool)),this,SLOT(handleShift()));

    ClickMenu.addAction("Insert Row",this,SLOT(handleInsertRow()),INSERTLINEABOVEKEY);
    ClickMenu.addAction("Copy",this,SLOT(handleCopy()));
    ClickMenu.addAction("Cut", this,SLOT(handleCut()));
    ClickMenu.addAction("Paste Over",this,SLOT(handlePasteOver()));
    ClickMenu.addAction(&shift);
    //    ClickMenu.addMenu(&QMenu("Hello"));
    selectedClickOptions(pos,&ClickMenu);
    if(Computer::getDefault()->connectedAddress(row)!=row)
    {
        ClickMenu.addAction("Go to Connected",this,SLOT(scrollToSelected()));
    }

    //    connect(shift,SIGNAL(triggered()),this, handleShift());
    ClickMenu.exec(mapToGlobal(pos  ));

}
void MemTable::paste(bool above)
{

}
void MemTable::pasteAbove()
{

}
void MemTable::selectedClickOptions(const QPoint &pos,QMenu* clickMenu)
{
    qDebug("am I in range?");
    int32_t first = (selectedIndexes().constFirst().row());
    int32_t last  = (selectedIndexes().constLast().row());
    std::cout<<first<<","<<last<<","<<indexAt(pos).row()<<std::endl;

    if((indexAt(pos).row() <= last )&& (indexAt(pos).row() >= first))
    {
        qDebug("Yes");
        QMenu* shiftMenu = new QMenu("Shift ",this);
        shiftMenu->addAction("Up", this, SLOT(handleShift()));
        shiftMenu->addAction("Down",this,SLOT(handleShift()));
        clickMenu->addMenu(shiftMenu);

    }

}
void MemTable::handleShift()
{
    bool b = 0;
    val_t start = selectedIndexes().constFirst().row();
    //    val_t start

    //       Computer::getDefault()->slideMemory(10,20,5,&b);
}
void MemTable::scrollToSelected()
{
    scrollToRow(Computer::getDefault()->connectedAddress(selectedIndexes().at(0).row()));
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

void MemTable::kick()
{
    hide();
    show();
}
void MemTable::handlePasteOver()
{
    qDebug("Handling Paste");
    mem_addr_t addr = this->selectedIndexes().at(0).row();

    Computer::getDefault()->setMemLoc(addr,*copied);
}
void MemTable::swap()
{
    if(model->currentMode==modeler::SelectMode::Cut)
    {
        bool b = 0;
        mem_addr_t begin = model->specialSelectStart;
        mem_addr_t end = model->specialSelectEnd;
        val_t delta = selectedIndexes().constFirst().row()-begin+1;
        //        Computer::getDefault()->slideMemory(model->specialSelectStart,model->specialSelectEnd,,delta,&b);
    }
}
void MemTable::setCut()
{
    qDebug("setCut");
    model->setSelectMode(modeler::SelectMode::Cut,selectedIndexes().constFirst().row(),
                         selectedIndexes().constLast().row());
    clearSelection();

}
void MemTable::setCopied()
{
    qDebug("setCopied");
    model->setSelectMode(modeler::SelectMode::Copy,
                         selectedIndexes().constFirst().row(),
                         selectedIndexes().constLast().row());
    clearSelection();


}
void MemTable::shiftUpBrute()
{
    shiftUp(false);
}
void MemTable::shiftDownBrute()
{
    qDebug("Shift Down Force");
    shiftDown(false);
}

void MemTable::shiftUp(bool makeAgreement)
{
    qDebug("shiftUp");
    bool b = false;
    if(selectedIndexes().size()>0)
    {
        QModelIndex original = currentIndex();

        mem_addr_t begin =  selectedIndexes().constFirst().row();
        mem_addr_t end =    selectedIndexes().constLast().row();

        Computer::getDefault()->slideMemory(begin,end,-1,makeAgreement,&b);


        setCurrentIndex(model->index(begin-1,original.column()));


        clearSelection();

        selectRange(begin-1,end-1);
    }


}
void MemTable::selectRange(mem_addr_t begin, int32_t end)
{

    QItemSelection * select = new QItemSelection(model->index(begin,0),model->index(end,model->columnCount()-1));
    selectionModel()->select(*select,QItemSelectionModel::Select);
}
void MemTable::shiftDown(bool makeAgreement)
{
    qDebug("shiftDown");
    bool b = false;

    if(selectedIndexes().size()>0)
    {
        QModelIndex original = currentIndex();
        qDebug("wor");
        mem_addr_t begin =  selectedIndexes().constFirst().row();
        mem_addr_t end =    selectedIndexes().constLast().row();
        Computer::getDefault()->slideMemory(begin,end,1,makeAgreement,&b);
        setCurrentIndex(model->index(begin+1,original.column()));
        clearSelection();
        selectRange(begin+1,end+1);
    }
}
void MemTable::hide()
{
    if(!isHidden())
    {
        savedCurrentFocusIndex = currentIndex();
        this->QTableView::hide();
    }
}
void MemTable::show()
{
    if(isHidden())
    {
        this->QTableView::show();
        if(savedCurrentFocusIndex==QModelIndex())
        {
            savedCurrentFocusIndex= currentIndex();
        }
        setCurrentIndex(savedCurrentFocusIndex);

    }
}
