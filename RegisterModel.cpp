#include "RegisterModel.h"
#include "Utility.h"
#include <QtGlobal>
#include <QTableView>
#include <QHeaderView>
#include <QBrush>
#include <QList>
#include <QColor>
#include <QVariant>
#include <QStandardItemModel>
#include <QUndoStack>
#include <QUndoCommand>
extern "C"{
#include "Util.h"
}
#include "HistoryHandler.h"
#include "Utility.h"
#include "computer.h"
RegisterModel::RegisterModel(QObject* parent,bool* excersize): QStandardItemModel(parent),threadRunning(excersize)
{
    qDebug("RegisterModel in progress");



    setColumnCount(REGCOLUMNCOUNT);
    setRowCount(12);



}

QVariant RegisterModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    return QStandardItemModel::headerData(section,orientation,role);
}

bool RegisterModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
    if (value != headerData(section, orientation, role))
    {
        // FIXME: Implement me!
        return true;
    }
    return false;
}

QModelIndex RegisterModel::index(int row, int column, const QModelIndex &parent) const
{
    return QStandardItemModel::index(row,column,parent);
}

QModelIndex RegisterModel::parent(const QModelIndex &index) const
{
    return QStandardItemModel::parent(index);
}

int RegisterModel::rowCount(const QModelIndex &parent) const
{
    return QStandardItemModel::rowCount(parent);
}

int RegisterModel::columnCount(const QModelIndex &parent) const
{
    return QStandardItemModel::columnCount(parent);
}

QVariant RegisterModel::data(const QModelIndex &index,int role) const
{


    int row = index.row();
    int column = index.column();
    if(!index.isValid()) return QVariant();
    if(role == Qt::CheckStateRole)
    {
        return  QVariant();
    }
    if(role == Qt::BackgroundRole)
    {
        if(column == reg_color_column)
        switch(row)
        {

            case R6:return R6COLOR;
            case R5:return R5COLOR;
            case R4:return R4COLOR;
            case R3:return R3COLOR;
            case R2:return R2COLOR;
            case PC:return PCCOLOR;
            case R1:return R1COLOR;
            case R0:return R0COLOR;
        }
        return QVariant();
    }

    if(role == Qt::DisplayRole|| role == Qt::EditRole)
    {
        switch(column)
        {
        case reg_color_column:
            return QString("");
        case reg_name_column :
            return regNameColumnHelper(row);
        case reg_value_column:
            return getHexString(Computer::getDefault()->getRegister(static_cast<reg_t>(row)));
        }
    }
    if(role == Qt::ToolTipRole)
    {
        if(column == reg_name_column)
        {
            switch(row)
            {
            case R0:
            case R1:
            case R2:
            case R3:return "General Purpose Register";
            case R6:return "User Stack Pointer";
            case R7:return "Return Register";
            case PC:return "Program Counter";
            case MPR:return "Memory Protection Register";
            case PSR:return "Program Status Register";
            case 11:return "Condition Code";
            }
        }
    }
    if(role == Qt::TextAlignmentRole)
    {
        return Qt::AlignLeft;
    }


    return QVariant();
}

bool RegisterModel::setData(const QModelIndex &index, const QVariant&value,int role)
{
     reg_t row = static_cast<reg_t>(index.row());
    int column = index.column();
     qDebug("Setting data for Register Model");
    if(!index.isValid())
    {
        return false;
    }
    switch(column)
    {
        case reg_color_column:qDebug("TBI");return false;
        case reg_name_column :qDebug("Isn't possible"); return false;
        case reg_value_column:
        if(row<=9)
        {
            qDebug("EDIT");

            // TODO I uncommented this cause I have no idea what the plan with this was
        //noTry->push(new Action::changeRegValue(row,QVARIANT2VAL_T(value)));
            bool* ok = (bool*)malloc(sizeof(bool));
            val_t newValue = Utility::unifiedInput2Val(value.toString(),ok);
            if(*ok)
            {
                Computer::getDefault()->setRegister(row,newValue);
            }
//            BATHTIME(CHARPTR2QSTRING(getHexString(getRegister(row))))
//            emit dataChanged(index,index,QVector<int>() <<role);
            return true;
        }
        if(row==9)
        {

        }
    }

 return false;
}

Qt::ItemFlags RegisterModel::flags(const QModelIndex&index) const
{
    if((*threadRunning) && threadRunning!=Q_NULLPTR)return 0;
    switch(index.column())
    {
        case reg_color_column:return Qt::ItemIsEnabled|Qt::ItemIsSelectable;
        case reg_name_column :return Qt::ItemIsEnabled|Qt::ItemIsSelectable;
        case reg_value_column:return Qt::ItemIsEnabled|Qt::ItemIsSelectable|Qt::ItemIsEditable;
    }

    return 0;
}

QString RegisterModel::regNameColumnHelper(int row) const
{
    switch(row)
    {
    case PC :return QString("PC");
    case PSR:return "PSR";
    case MPR:return "MPR";
    case 11: return "CC";
    default :return QString("R").append(QString().setNum(row));
    }
    return "Error";
}

void RegisterModel::update()
{

}
QList<QColor>* RegisterModel::getRegColors(){
    return regColors;
}
cond_t RegisterModel::handle_CC_RegisterView_Input(QString in)
{
    qDebug("Handling Conditional");
    QString lowered = in.toLower();
    qDebug("Checking if it is a register");
    if(lowered.startsWith("r"))
    {
        qDebug("It might be");
        if(lowered.length()==2)
        {
            qDebug("It looks like it");
            QString ch = lowered.at(1);
            int reg_num =0;
            ch.number(reg_num,16);
            if(reg_num<9)
            {
                qDebug("It really is!");
                val_t val = Computer::getDefault()->getRegister(static_cast<reg_t>(reg_num));
                if(val&0x0)return cond_n;
                if(val>0)return cond_p;
                if(val==0)return cond_z;
            }
            qDebug("Nope");
        }
        qDebug("Nadda");
    }
    //Handle negative
    PSR_HANDLER(lowered,"n"     ,   cond_n)
    PSR_HANDLER(lowered,"negative" ,cond_n)
    PSR_HANDLER(lowered,":("    ,   cond_n)
    PSR_HANDLER(lowered,": ("   ,   cond_n)
    PSR_HANDLER(lowered,":-("   ,   cond_n)
    PSR_HANDLER(lowered,"neg"   ,   cond_n)
    PSR_HANDLER(lowered,"en"    ,   cond_n)
    PSR_HANDLER(lowered,"nn"    ,   cond_n)
    PSR_HANDLER(lowered,"nnn"   ,   cond_n)
    PSR_HANDLER(lowered,"-"     ,   cond_n)
    //Handle positive
            PSR_HANDLER(lowered,"😀",cond_p)
    PSR_HANDLER(lowered,"p"     ,   cond_p)
    PSR_HANDLER(lowered,"positive" ,cond_p)
    PSR_HANDLER(lowered,":)"    ,   cond_p)
    PSR_HANDLER(lowered,": )"   ,   cond_p)
    PSR_HANDLER(lowered,":-)"   ,   cond_p)
    PSR_HANDLER(lowered,"pos"   ,   cond_p)
    PSR_HANDLER(lowered,"po"    ,   cond_p)
    PSR_HANDLER(lowered,"pp"    ,   cond_p)
    PSR_HANDLER(lowered,"ppp"   ,   cond_p)
    PSR_HANDLER(lowered,"+"     ,   cond_p)
    //Handle zero
    PSR_HANDLER(lowered,"z"     ,   cond_z)
    PSR_HANDLER(lowered,"zero"  ,   cond_z)
    PSR_HANDLER(lowered,":|"    ,   cond_z)
    PSR_HANDLER(lowered,": |"   ,   cond_z)
    PSR_HANDLER(lowered,":-|"   ,   cond_z)
    PSR_HANDLER(lowered,"zer"   ,   cond_z)
    PSR_HANDLER(lowered,"ze"    ,   cond_z)
    PSR_HANDLER(lowered,"zz"    ,   cond_z)
    PSR_HANDLER(lowered,"zzz"   ,   cond_z)
    PSR_HANDLER(lowered,""      ,   cond_z)


    if(lowered.toInt()>0)return     cond_p;
    if(lowered.toInt()<0)return     cond_n;


    return cond_z;
}





