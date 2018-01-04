#include "StackModeler.h"
#include "Utility.h"
#include "QBrush"

using namespace Stack_Modulos;
StackModeler::StackModeler(QObject* parent,bool* access):modeler(parent,access)
{
    qDebug("StackView Initialzing");
    QStringList HeaderLabel;
    HeaderLabel.append("Addr");
    HeaderLabel.append("Offset");
    setHorizontalHeaderLabels(HeaderLabel);
    setRowCount(0xC000 - 0x3000);
    qDebug("Current num of rows is" + QString().setNum(rowCount()).toLocal8Bit());
}
QVariant StackModeler::data(const QModelIndex &index,int role) const
{
    mem_addr_t addr;
    if(*bigTop){
    addr = 0xBFFF - index.row();
    }else
    {
        addr = 0x3000 + index.row();
    }

    const int column = index.column();
    if(!index.isValid())return QVariant();
    if(role == Qt::DisplayRole)
    {
        switch(column)
        {
        case ADDRCOLUMN:
            return QVariant(getHexString(addr));
        case OFFSETCOLUMN:
            return QVariant((abs(addr - Computer::getDefault()->getRegister(R6)) < 256) ? QString().setNum(addr - Computer::getDefault()->getRegister(R6)):"");

        case VALUECOLUMN:
            return QVariant(getHexString(Computer::getDefault()->getMemValue(addr)));
        }
    }
    if(role == Qt::BackgroundRole)
    {
       if(column==0)
       {
           return rowPainter(addr);
       }
    }
    return QVariant();
}
void StackModeler::setData(const QModelIndex &index, const QVariant &value, int role) const
{
    bool ok = true;
    mem_addr_t addr;
    if(*bigTop){
    addr = 0xBFFF - index.row();
    }else
    {
        addr = 0x3000 + index.row();
    }

    const int column = index.column();
    if(role == Qt::EditRole)
    {
        switch(column){
        case NAMECOLUMN:qDebug("You just set a Label");
        {
            Computer::getDefault()->setMemLabelText(addr,value.toString());
        }
        case VALUECOLUMN:
        {
            val_t newValue = Utility::unifiedInput2Val(value.toString(),&ok);
            if(ok)
            {
                Computer::getDefault()->setMemValue(addr,newValue);
            }
        }
        }




    }
}




Qt::ItemFlags StackModeler::flags(const QModelIndex &index) const
{
    switch(index.column())
    {
    case ADDRCOLUMN:return Qt::ItemIsEnabled;
    case OFFSETCOLUMN:return Qt::ItemIsEnabled;
    case NAMECOLUMN:return Qt::ItemIsEnabled|Qt::ItemIsEditable;
    case VALUECOLUMN:return Qt::ItemIsEnabled|Qt::ItemIsEditable;
    }

    return Qt::ItemIsEnabled;
}
void StackModeler::flip()
{
    qDebug("Flippin'");
    bigTop = new bool(!(*bigTop));
    IFNOMASK(emit update();)
}
void StackModeler::setTopBig(bool ghost)
{
    bigTop = new bool(ghost);
}
