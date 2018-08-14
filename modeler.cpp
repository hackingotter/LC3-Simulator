#include "modeler.h"

#include <QtGlobal>
#include <QVariant>
#include <QStandardItemModel>
#include <QUndoStack>
#include "Utility.h"
#include "HistoryHandler.h"
#include "Bridge.h"
#include <QString>
#include "Utility.h"
#include "opcodes.h"
#include "iostream"
extern "C"
{
#include <string.h>
#include <string>
#include <Memory.h>
#include <Util.h>
//#include <Simulator.h>
}
#include <QItemSelectionModel>

#define TESTOFFSET 1
#define TESTBEGIN  10
#define TESTEND    10

using namespace Memory_Modulos;
modeler::modeler(QObject *parent,bool* access): QStandardItemModel(parent),threadRunning(access)
{
    QStringList HeaderLabel;
    //Since there is no real easy way of setting the headers, I fill it up with "EMPTY"
    //header items, then proceed to replace each
    for(int i = 0;i<NUMBER_OF_COLUMNS;i++)
    {
        HeaderLabel.append("EMPTY");
    }
    HeaderLabel.replace(BRCOLUMN  ,"BR");
    HeaderLabel.replace(ADDRCOLUMN,"Addr");
    HeaderLabel.replace(VALUCOLUMN,"Value");
    HeaderLabel.replace(NAMECOLUMN,"Name");
    HeaderLabel.replace(COMMCOLUMN,"Comments");
    HeaderLabel.replace(MNEMCOLUMN,"Mnem");
    setHorizontalHeaderLabels(HeaderLabel);
    setItem(0,new QStandardItem("Hey"));
    setRowCount(MEMSIZE+1);


}



QVariant modeler::headerData(int section, Qt::Orientation orientation, int role) const
{
    return QStandardItemModel::headerData(section,orientation,role);
}

bool modeler::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
    if (value != headerData(section, orientation, role))
    {
        // FIXME: Implement me!
        emit headerDataChanged(orientation, section, section);
        return true;
    }
    return false;
}

QModelIndex modeler::index(int row, int column, const QModelIndex &parent) const
{
    if(this->isFlipped)
    {
        row = 0xBFFF-row;
    }
    return QStandardItemModel::index(row,column,parent);
}



int modeler::rowCount(const QModelIndex &parent) const
{
    return QStandardItemModel::rowCount(parent);
}

int modeler::columnCount(const QModelIndex &parent) const
{
    return QStandardItemModel::columnCount(parent);
}

QBrush modeler::column0Painter(mem_addr_t addr) const
{
    if(addr == Computer::getDefault()->getRegister(PC))
    {

        return QBrush(PCCOLOR);
    }
    if(addr == Computer::getDefault()->getRegister(R7))
    {

        return QBrush(Qt::blue);
    }
    if(addr == Computer::getDefault()->getRegister(R1))
    {
        return QBrush(R1COLOR);
    }
    if(addr == Computer::getDefault()->getRegister(R2))
    {
        return QBrush(R2COLOR);
        //            darkRed,
        //            darkGreen,
        //            darkBlue,
        //            darkCyan,
    }
    if(addr == Computer::getDefault()->getRegister(R3))
    {
        return QBrush(R3COLOR);
    }
    if(addr == Computer::getDefault()->getRegister(R4))
    {
        return QBrush(R4COLOR);
    }
    if(addr == Computer::getDefault()->getRegister(R5))
    {
        return QBrush(R5COLOR);
    }
    if(addr == Computer::getDefault()->getRegister(R6))
    {
        return QBrush(R6COLOR);
    }
    //the below doesn't work
    val_t pc = Computer::getDefault()->getRegister(PC);

    if(addr == Computer::getDefault()->connectedAddress(pc))
    {
        return QBrush(QColor(230,230,230));
    }
    return QBrush();
}

QBrush modeler::column3Painter(mem_addr_t addr) const
{
    return QBrush();//TBI
}

void modeler::setSelectMode(modeler::SelectMode mode, mem_addr_t begin, mem_addr_t end)
{
    this->currentMode = mode;
    Utility::Utilit::specialSelectStart = begin;
    Utility::Utilit::specialSelectEnd = end;

    emit change();
}

void modeler::setIfMNemIsDisplayedAfterDataType(bool display)
{
    displayMNemonicAfterDataType =  display;
}
QBrush modeler::rowPainter(mem_addr_t addr,const QModelIndex &index) const
{


    switch(index.column())
    {
    case 0: return column0Painter(addr);break;
    case 1:
    {
        if(Utility::Utilit::specialSelectStart<=Utility::Utilit::specialSelectEnd)
            if(Computer::getDefault()->isBetween(Utility::Utilit::specialSelectStart,Utility::Utilit::specialSelectEnd,addr))
            {
                switch(currentMode)
                {
                case SelectMode::Copy:
                    return QBrush(QColor(100,235,213));
                case SelectMode::Cut:
                    return QBrush(QColor(12,230,20));
                default:

                    return QBrush(QColor(157,157,20));
                }
            }


    }break;
    case 2:
    {
        //        for(int i = 0; i < 30; i++)
        //        {


        //            int value = Computer::getDefault()->proposedNewLocation(i,TESTBEGIN,TESTEND,TESTOFFSET);
        //            if(addr == value)
        //            {

        //                if(addr == i)
        //                {
        //                    return QBrush(QColor(250,100,100));
        //                }
        //                else
        //                if(addr > i)
        //                {
        //                    return QBrush(QColor(100,220,100));
        //                }
        //                else
        //                if(addr < i)
        //                {
        //                    return QBrush(QColor(100,100,220));
        //                }

        //            }
        //            if(Computer::getDefault()->connectedAddress(addr)==value)
        //            {
        //                return QBrush(QColor(150,150,200));
        //            }


        //        }
    }
    case VALUCOLUMN:
    {
        if(Computer::getDefault()->getMemDataType(addr) == COLOR)
        {
            return QBrush(Utility::int2QColor(Computer::getDefault()->getMemValue(addr)));
        }
    }

    }





    //    if(addr == copied->addr)
    //    {
    //        return QBrush(QColor(250,30,205));
    //    }
    return QBrush();
}

void modeler::setCopied(QModelIndexList *target)
{
    copiedLength = target->length();
    copied = Computer::getDefault()->getMemLocationsBlock(target->at(0).row(),copiedLength);
    emit change();
}

QVariant modeler::handleDataValueColumn(mem_addr_t addr,int role) const
{
    switch(role)
    {
        case Qt::CheckStateRole: return QVariant();
    case Qt::BackgroundRole:
    {
        switch (Computer::getDefault()->getMemDataType(addr)) {
        case COLOR:
            //TBI
            break;
        default:
            break;
        }
    }
//    case Qt::DisplayRole:
//        return
    }
return "ABCDEFG";
}

QVariant modeler::data(const QModelIndex &index, int role) const
{

    int column = index.column();
    mem_addr_t addr = index.row();
    switch(role)
    {
    case Qt::CheckStateRole:return dataCheck(index);
    case Qt::BackgroundRole:return dataBack(index);
    }
    if(role == Qt::CheckStateRole)
    {
        if(column == BRCOLUMN)
        {
            breakpoint_t va = Computer::getDefault()->getMemBreakPoint(addr);
            return QVariant(va != nullptr);
        }
        return  QVariant();
    }
    if(role == Qt::DisplayRole)
    {


        //        BATHTIME("E")
        switch(column)
        {
        case ADDRCOLUMN:
            return getHexString(addr);
            //        case BRCOLUMN:return /*(bool)getMemBreakPoint(addr);*/ 0;
        case VALUCOLUMN:
            //            return getHexString(Computer::getDefault()->proposedNewLocation(addr,TESTBEGIN,TESTEND,TESTOFFSET));

            return getHexString(Computer::getDefault()->getMemValue(addr));
        case NAMECOLUMN:
        {
            return Computer::getDefault()->getMemNameSafe(addr);
        }
        case MNEMCOLUMN:
            return Computer::getDefault()->displayAddressValue(addr,displayMNemonicAfterDataType);
//            return Computer::getDefault()->displayData(addr);
        case COMMCOLUMN:
            return Computer::getDefault()->getMemComment(addr);
        }

    }
    if(role == Qt::EditRole)
    {
        switch (column) {
        case VALUCOLUMN:
        {
            return getHexString(Computer::getDefault()->getMemValue(addr));
        }
        case NAMECOLUMN:
        {
            return Computer::getDefault()->getMemNameSafe(addr);
        }
        case COMMCOLUMN:
        {
            return Computer::getDefault()->getMemComment(addr);
        }
        default:
            break;
        }
        return QVariant();
    }

    return QVariant();
}

QVariant modeler::dataCheck(const QModelIndex &index)const
{
    int column = index.column();
    mem_addr_t addr = index.row();

    if(column == BRCOLUMN)
    {
        breakpoint_t va = Computer::getDefault()->getMemBreakPoint(addr);
        return QVariant(va != nullptr);
    }
    return  QVariant();
}
QVariant modeler::dataBack(const QModelIndex &index)const
{
    //    int column = index.column();
    mem_addr_t addr = index.row();

    return rowPainter(addr,index);
}
bool modeler::setData(const QModelIndex &index, const QVariant &value, int role)
{
    bool ok = true;
    qDebug("Setting Data");
    mem_addr_t addr = index.row();
    int column = index.column();
    qDebug("Am I a valid index?");
    if(index.isValid())
    {
        switch(column)
        {
        case BRCOLUMN  :qDebug("You just set a breakpoint");
            qDebug("TBI");
            if(Computer::getDefault()->getMemBreakPoint(addr) == nullptr)
            {
                Computer::getDefault()->setMemBreakPoint(addr,(void **)((void *)1));
            }
            else
            {
                Computer::getDefault()->setMemBreakPoint(addr,(void **)(nullptr));
            }
            emit breakChanged(addr,nullptr);
            return true;
        case ADDRCOLUMN:qDebug("There must be an error"); return false;
        case NAMECOLUMN:qDebug("You just set a Label");
        {

            Computer::getDefault()->setMemLabelText(addr,value.toString());

        }
            return true;
        case VALUCOLUMN:
            qDebug("You just set a Value");
        {

            val_t newValue = Utility::unifiedInput2Val(value.toString(),&ok);
            if(ok)
            {
                Computer::getDefault()->setMemValue(addr,newValue);
            }


        }
            return true;
        case COMMCOLUMN:qDebug("You just set a Comment");

        {
            QString newComment = value.toString();
            Computer::getDefault()->setMemComment(addr,newComment);
            emit commentChanged(addr,newComment);
        }
            return true;
        case MNEMCOLUMN:qDebug("You just set a mnemonic");qDebug("TBI");return true;
        }


    }
    emit dataChan(addr);
    emit layoutChanged();
    emit dataChanged(index, index, QVector<int>() << role);
    emit itemChanged(item(addr,column));
    emit change();
    return true;

}


Qt::ItemFlags modeler::flags(const QModelIndex &index) const
{
    if((*threadRunning) && threadRunning!=Q_NULLPTR)return 0;
    switch(index.column())
    {
    case BRCOLUMN  :return Qt::ItemIsEnabled|Qt::ItemIsSelectable|Qt::ItemIsUserCheckable|Qt::ItemIsTristate;
    case ADDRCOLUMN:return Qt::ItemIsEnabled|Qt::ItemIsSelectable;
    case NAMECOLUMN:return Qt::ItemIsEnabled|Qt::ItemIsEditable|Qt::ItemIsSelectable|Qt::ItemIsDragEnabled|Qt::ItemIsDropEnabled|Qt::ItemIsDropEnabled;
    case VALUCOLUMN:return Qt::ItemIsEnabled|Qt::ItemIsEditable|Qt::ItemIsSelectable;
    case COMMCOLUMN:return Qt::ItemIsEnabled|Qt::ItemIsEditable|Qt::ItemIsSelectable;
    case MNEMCOLUMN:return Qt::ItemIsEnabled|Qt::ItemIsEditable|Qt::ItemIsSelectable;
    }
    return 0;

    //    switch(index.column()){
    //    case BRCOLUMN   :return Qt::ItemIsUserCheckable+Qt::ItemIsEnabled;
    //    case ADDRCOLUMN :return Qt::NoItemFlags;
    //    case VALUCOLUMN :return Qt::ItemIsEditable;
    //    }

}
void modeler::update()
{

}


