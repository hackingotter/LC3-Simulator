#include "modeler.h"

#include <QtGlobal>
#include <QVariant>
#include <QStandardItemModel>
#include <QUndoStack>
#include "Utility.h"
#include "HistoryHandler.h"
#include "Bridge.h"
extern "C"
{
#include <string.h>
#include <string>
#include <Memory.h>
#include <Util.h>
#include <Simulator.h>
}



using namespace Memory_Modulos;
modeler::modeler(QObject *parent,bool* access): QStandardItemModel(parent),threadRunning(access)
{
    BATHTIME("THERE")
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


void modeler::setQUndoStack(QUndoStack* doOrUndo)
{
    noTry = doOrUndo;
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
    if (role == Qt::DisplayRole || role == Qt::EditRole)
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
            if (Computer::getDefault()->getMemLabel(addr)!= nullptr)
            {
                return Computer::getDefault()->getMemLabel(addr)->name;
            }
            return "";
        }

        case MNEMCOLUMN:
            return Utility::addr2Mnem(addr);
        case COMMCOLUMN:
            return Computer::getDefault()->getMemComment(addr);
        }
    }
    return QVariant();
}

bool modeler::setData(const QModelIndex &index, const QVariant &value, int role)
{
    BATHTIME("Setting Data")
            bool success = true;//this will change when success isn't found
    mem_addr_t addr = index.row();
    int column = index.column();
    BATHTIME("Am I a valid index?")
            if(index.isValid())
    {
        switch(column)
        {
        case BRCOLUMN  :BATHTIME("You just set a breakpoint")
                    BATHTIME("TBI");

            Computer::getDefault()->setMemBreakPoint(addr,NULL);
            emit breakChanged(addr,nullptr);
            return true;
        case ADDRCOLUMN:BATHTIME("There must be an error") return false;
        case NAMECOLUMN:BATHTIME("You just set a Label")
            {

                Computer::getDefault()->setMemLabelText(addr,value.toString());
            }


            return true;
        case VALUCOLUMN:
            BATHTIME("You just set a Value")
            {
                BATHTIME(value.toString())
                        QString valString = value.toString();
                BATHTIME(valString)
                        if(value.toString().startsWith("x")) valString = valString.remove(0,1);
                BATHTIME(valString)
                        bool ok = false;
                val_t val = static_cast<val_t>(valString.toInt(&ok,16));
                if(!ok)
                {
                    BATHTIME("NOPE")
                }else
                {


                    BATHTIME("Requesting Change")
                            noTry->push(new Action::changeMemValue(addr,val));
                    BATHTIME("hey")
                            //                        emit update();
                            //                setMemValue(addr,val);
                            //                emit valueChanged(addr,val);
                }
            }
            return true;
        case COMMCOLUMN:BATHTIME("You just set a Comment")

            {
                QString newComment = value.toString();
                Computer::getDefault()->setMemComment(addr,newComment);
                emit commentChanged(addr,newComment);
            }
            return true;
        case MNEMCOLUMN:BATHTIME("You just set a mnemonic")BATHTIME("TBI");return true;
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


    out = Utility::addr2Mnem(instruction);
    return out;
}

