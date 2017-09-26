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
extern "C"
{
#include <string.h>
#include <string>
#include <Memory.h>
#include <Util.h>
//#include <Simulator.h>
}



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


    HeaderLabel.replace(BRCOLUMN,"BR");
    HeaderLabel.replace(ADDRCOLUMN,"Addr");
    HeaderLabel.replace(VALUCOLUMN,"Value");
    HeaderLabel.replace(NAMECOLUMN,"Name");
    HeaderLabel.replace(COMMCOLUMN,"Comments");
    HeaderLabel.replace(MNEMCOLUMN,"Mnem");
    setHorizontalHeaderLabels(HeaderLabel);
    setItem(0,new QStandardItem("Hey"));
    setRowCount(MEMSIZE);
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

QVariant modeler::data(const QModelIndex &index, int role) const
{

    int column = index.column();
    mem_addr_t addr = index.row();
    if(role == Qt::CheckStateRole)
    {
        if(column == BRCOLUMN) return 0;
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
    if(role == Qt::BackgroundRole&&column == 0)
    {

        if(addr == Computer::getDefault()->getRegister(PC))
        {

            return QBrush(Qt::yellow);
        }
        if(addr == Computer::getDefault()->getRegister(R7))
        {

            return QBrush(Qt::blue);
        }
        return QVariant();
    }
    if (role == Qt::DisplayRole)
    {


        //        BATHTIME("E")
        switch(column)
        {
        case ADDRCOLUMN:
            return getHexString(addr);
            //        case BRCOLUMN:return /*(bool)getMemBreakPoint(addr);*/ 0;
        case VALUCOLUMN:
            return getHexString(Computer::getDefault()->getMemValue(addr));
        case NAMECOLUMN:
        {

            label_t* l = Computer::getDefault()->getMemLabel(addr);
            if (l)
                return (l->name);
            return "N/A";

        }

        case MNEMCOLUMN:
            //            qDebug("ehlo");
            //return ""; // TODO this is not working. It get's stuck
            return QString(addr2Mnem(addr));
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
        default:
            break;
        }
        return QVariant();
    }
    return QVariant();
}

bool modeler::setData(const QModelIndex &index, const QVariant &value, int role)
{
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

            Computer::getDefault()->setMemBreakPoint(addr,NULL);
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
            QString valString = value.toString();
            if(value.toString().startsWith("x")) valString = valString.remove(0,1);
            bool ok = false;
            val_t val = static_cast<val_t>(valString.toInt(&ok,16));
            if(!ok)
            {
                qDebug("NOPE");
            }else
            {


                qDebug("Requesting Change");
                Computer::getDefault()->setMemValue(addr,val);
                qDebug("hey " + QString().setNum(val).toLocal8Bit());

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
QString modeler::mnemonicGen(mem_addr_t addr) const
{
    QString out;
    val_t instruction = Computer::getDefault()->getMemValue(addr);
    //    BATHTIME(getHexString(instruction>>12))
    //    BATHTIME(getHexString(andOpCode>>12))


    out = addr2Mnem(instruction);
    return out;
}
QString modeler::addr2Mnem(mem_addr_t addr) const
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
            mem_addr_t target = addr +( val & 0x00FF);
            label_t* label = Computer::getDefault()->getMemLabel(target);
            out.append(" ");
            if(label != nullptr)
            {
                out.append(getHexString(target));
            }
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
        out.append("R" + QSTRNUM(reg11) + ", "+ name_or_addr(addr+val & 0x00FF));
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


