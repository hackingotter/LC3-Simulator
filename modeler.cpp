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

#define TESTOFFSET -1
#define TESTBEGIN  10
#define TESTEND    10

using namespace Memory_Modulos;
modeler::modeler(QObject *parent,bool* access): QStandardItemModel(parent),threadRunning(access)
{
    QStringList HeaderLabel;
    //Since there is no real easy way of setting the headers, i fill it up with "EMPTY"
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

    if(addr == Computer::getDefault()->connectedAddress(PC))
    {
        return QBrush(QColor(230,230,230));
    }
    return QBrush();
}

void modeler::setSelectMode(modeler::SelectMode mode, mem_addr_t begin, mem_addr_t end)
{
    this->currentMode = mode;
    Utility::Utilit::specialSelectStart = begin;
    Utility::Utilit::specialSelectEnd = end;


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
        for(int i = 0; i < 30; i++)
        {


            int value = Computer::getDefault()->proposedNewLocation(i,TESTBEGIN,TESTEND,TESTOFFSET);
            if(addr == value)
            {

                if(addr == i)
                {
                    return QBrush(QColor(250,100,100));
                }
                else
                if(addr > i)
                {
                    return QBrush(QColor(100,220,100));
                }
                else
                if(addr < i)
                {
                    return QBrush(QColor(100,100,220));
                }
            }


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
    //    if(role == Qt::TextAlignmentRole){
    //        switch (column) {

    //        case NAMECOLUMN:return Qt::AlignRight;
    //        case VALUCOLUMN:return Qt::AlignCenter;
    //        }
    //    }
    /*
     * Testing if it is asking for the background color
     */

    if (role == Qt::DisplayRole)
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
            if(addr<30)
            {
                QString* er = new QString();
                Computer::getDefault()->proposedNewLocation(addr,TESTBEGIN,TESTEND,TESTOFFSET,er);
                return *er;
            }

            return Computer::getDefault()->getMemNameSafe(addr);

        }

        case MNEMCOLUMN:
            return this->addr2Mnem(addr);
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
QString modeler::mnemonicGen(mem_addr_t addr)const
{
    QString out;
    val_t instruction = Computer::getDefault()->getMemValue(addr);
    //    BATHTIME(getHexString(instruction>>12))
    //    BATHTIME(getHexString(andOpCode>>12))


    out = addr2Mnem(instruction);
    return out;
}


QString modeler::addr2Mnem(mem_addr_t addr)const
{
    //    qDebug(QString().setNum(addr).toLocal8Bit());

    val_t val = Computer::getDefault()->getMemValue(addr);
    addr +=1;
    QString out;
    int imm5        =(val&0x001F);
    int reg11       =((val&0x0E00) >> 9);
    int reg8        =((val&0x01E0) >> 6);
    int reg2        =((val&0x0007) >> 0);
    //    bool zero543    =(val&0x0038);
    bool imm5YN     =(val&0x0020) >> 5;

    //It is much easier to be able to handle them without needing to account for names
    switch(val&0xF000)
    {
    case andOpCode  :out.append("AND ");break;
    case addOpCode  :out.append("ADD ");break;
    case brOpCode   :out.append("BR");break;
    case jmpOpCode  :out.append("JMP ");break;
    case jsrOpCode  :out.append("JSR");break;
    case ldOpCode   :out.append("LD ");break;
    case ldiOpCode  :out.append("LDI ");break;
    case ldrOpCode  :out.append("LDR ");break;
    case leaOpCode  :out.append("LEA ");break;
    case notOpCode  :out.append("NOT ");break;
    case rtiOpCode  :out.append("RTI ");break;
    case stOpCode   :out.append("ST ");break;
    case stiOpCode  :out.append("STI ");break;
    case strOpCode  :out.append("STR ");break;
    case trapOpCode :out.append("TRAP ");break;
    }

    switch(val& 0xF000)
    {
    case addOpCode:
        if((val&0x0038) ==0x0010)
        {
            out = QString("SUB ");

        }

    case andOpCode:
    {
        if((((val&0x0020)&&(val&0x0008))||(val&0x0020))) out = BADOP;
        else
        {
            out.append("R" + QSTRNUM(reg11) + ", R"+QSTRNUM(reg8));
            if(imm5YN)
            {
                out.append(", #").append(QSTRNUM(imm5));
            }
            else
            {
                out.append(", R").append(QSTRNUM(reg2));
            }
        }
        break;
    }
    case brOpCode :
    {
        if((val&0x0E00)==0x0E00)//if all three are set, just display BR
        {}
        else
            if(val&0x0E00)
            {
                if(val&0x0800) out.append("n");
                if(val&0x0400) out.append("z");
                if(val&0x0200) out.append("p");
            }
        if((val&0x0E00)==0x0000)
        {
            out = BADOP;
        }
        else
        {
            val_t offset = val&0x01FF;
            if(val&0x0100) val|=0xFE00;
            mem_addr_t target = addr + offset;
            out.append(" ");
            out.append(name_or_addr(target));
        }
        break;

    }
    case jmpOpCode:
    {
        if((val&0x0E3F))//if there are ones outside of the OpCode and BaseR
            //Bad Op
        {
            out = BADOP;
        }
        else if(val&0x01C0)//if so, RET is the proper memn
        {
            out = "RET";
        }
        else
        {
            out = QString("JMP R%1").arg(reg8);
        }

        break;
    }
    case jsrOpCode:
    {
        qDebug("JSR");
        if(val&0x0800)//11th slot 1 means jsr
        {
            val_t offset = val& 0x07FF;

            if(offset&0x400)offset |= 0xF400;
            mem_addr_t target = addr + (val & 0x07FF);
            out.append(" " + name_or_addr(target));
        }
        else if(!(val&0x0E3F))//or, it could be jsrr
        {
            out.append("R R"+QSTRNUM(reg8));
        }
        else
        {
            out = BADOP;
        }
        break;
    }
    case ldOpCode :
    case ldiOpCode:
    case leaOpCode:
    case stOpCode :
    case stiOpCode:
    {
        val_t offset = val&0x01FF;
        if(val&0x0100) val|=0xFE00;

        val_t target = addr+offset;
        out.append("R" + QSTRNUM(reg11) + ", "+ name_or_addr(target));
        break;
    }
    case ldrOpCode:
    case strOpCode:
    {
        out.append("R"+ QSTRNUM(reg11)+", R"+ QSTRNUM(reg8)+ ", "+QSTRNUM(val&0x003F));
        break;
    }
    case rtiOpCode:
    {if(val&0x0FFF) out = BADOP;break;}
    case trapOpCode:
    {
        if(val&0x0F00)
        {
            out = BADOP;
        }
        else
        {
            name_or_addr(val&0x00FF);//TODO this code could be wrong
        }
        break;
    }
    }
    return out;
}


QString modeler::name_or_addr(mem_addr_t target) const
{
    label_t* label = Computer::getDefault()->getMemLabel(target);

    if(label!= nullptr)
    {
        return QString(label->name);
    }
    else
    {
        return  getHexString(target);
    }
    return "";
}


