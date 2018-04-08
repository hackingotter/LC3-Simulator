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
    stackFrameColors = new val_t[0xC000 - 0x3000];
//    for(val_t i = 0; i<0xff;i++)
//    {
//        qDebug(getHexString(colorHash(i)).toLocal8Bit());
//    }
    qDebug("Current num of rows is" + QString().setNum(rowCount()).toLocal8Bit());

}
QVariant StackModeler::data(const QModelIndex &index,int role) const
{
    mem_addr_t addr;

    addr = 0xBFFF - index.row();

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
            return QVariant(stackFrameColors[index.row()]/*Computer::getDefault()->getMemValue(addr)*/);
        }
    }
    if(role == Qt::BackgroundRole)
    {
        if(column==0)
        {
            return rowPainter(addr,index);
        }
        if(column == VALUECOLUMN)
        {
            return stackFramePainter(0xBFFF-index.row());
        }
    }
    return QVariant();
}
void StackModeler::stackFrameListener(mem_addr_t addr)
{
    if(addr<=0xBFFF && addr>=0x3000)
    {
        qDebug("jel");
//        qDebug(getHexString(colorHash(Computer::getDefault()->getRegister(R7))).toLocal8Bit());
        stackFrameColors[addr-0x3000]=colorHash(Computer::getDefault()->getRegister(R7));
    }
}
QBrush StackModeler::stackFramePainter(mem_addr_t addr) const

{
    int c = 256-( stackFrameColors[addr-0x3000]%255);
//    int c = addr%256;
    return QBrush(QColor(100+(6*c%6),(200-9*(c%17))%255,(31*(c^5))%255));
}
val_t StackModeler::colorHash(val_t val)
{
    val_t out = 0;
    out+=val<<8;
    out+=val>>8;
       qDebug(getHexString(out).toLocal8Bit());
    val_t temp = out<<2;
    temp+=out>>14;
    out= temp;

    if(out&0x0400)
    {
        temp = out>>3;
        out = (out<<3) + temp;
    }
    else
    {
        temp = out<<3;
        out = (out>>3) + temp;
    }
    if(out&0x0010)
    {
        temp = out>>5;
        out = (out<<5) + temp;
    }
    else
    {
        temp = out<<2;
        out = (out>>2) + temp;
    }
    qDebug(getHexString(out).toLocal8Bit());
    return out;
}

bool StackModeler::setData(const QModelIndex &index, const QVariant &value, int role)

{
    static int a = 0;
    bool ok = true;
    mem_addr_t addr;
    addr = 0xBFFF - index.row();
    qDebug(getHexString(addr).toLocal8Bit());
    qDebug(QString().setNum(a++).toLocal8Bit());
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
                stackFrameListener(addr);
            }

        }
        }
    }
    return true;
}


QModelIndex StackModeler::index(int row, int column, const QModelIndex &parent) const
{

    return QStandardItemModel::index(row,column,parent);
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
