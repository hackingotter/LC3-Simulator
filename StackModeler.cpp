#include "StackModeler.h"
#include "Utility.h"
#include "QBrush"

using namespace Stack_Modulos;
StackModeler::StackModeler(QObject* parent,bool* access):modeler(parent,access)
{
    stackFrameColorCounter = 0;
    qDebug("StackView Initialzing");
    QStringList HeaderLabel;
    HeaderLabel.append("Addr");
    HeaderLabel.append("Offset");
    setHorizontalHeaderLabels(HeaderLabel);
    setRowCount(0xC000 - 0x3000);
    QObject::connect(Computer::getDefault(),SIGNAL(memValueChanged(mem_addr_t)),this,SLOT(stackFrameListener(mem_addr_t)));
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

    addr = STACKSPACE_END - index.row();

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
        case NAMECOLUMN:
            return QVariant(Computer::getDefault()->getMemNameSafe(addr));
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
            return stackFramePainter(addr);
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
        stackFrameColors[addr-0x3000]=colorHash(Computer::getDefault()->getRegister(R6));
    }
}

void StackModeler::increaseStackFrameCounter()
{
    stackFrameColorCounter++;
}
void StackModeler::decreaseStackFrameCounter()
{
    stackFrameColorCounter--;
}
QBrush StackModeler::stackFramePainter(mem_addr_t addr) const

{
    int c = 256-((( stackFrameColors[addr-0x3000])^37)%255);
//    int c = addr%256;
    //there is no real rhythm or rhyme to this, it just psuedo randomizes the color a stack frame will get.
    return QBrush(QColor(50+(((67*6*(c%29))^3)%206),50+((200-9*(c%17))%206),50+((89*(c^5))%206)));
    return QBrush(QColor(c,c,c));
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
    addr = STACKSPACE_END - index.row();
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
