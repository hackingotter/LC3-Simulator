#include "WatchModel.h"
#include "modeler.h"
#include "QModelIndex"
#include <QAbstractItemModel>
WatchModel::WatchModel(QObject* parent, bool *access):modeler(parent,access)
{
    Offset =new QVector<val_t>();
    Offset->append(4);
    Offset->append(19);
    Offset->append(9);
    Offset->append(10);

    setRowCount(4);
    setColumnCount(modeler::columnCount()+3);

}
QVariant WatchModel::data(const QModelIndex &index, int role) const
{
    QModelIndex offsetIndex = createIndex((int)(Offset->at(index.row())),index.column(),index.internalPointer());




    return modeler::data(offsetIndex,role);
}
bool WatchModel::handleAddresColumn(const QModelIndex &index, const QVariant &value, int role)
{
    bool a;
    switch(role)
    {
    case Qt::EditRole:
    {
        val_t possibleEntry = Utility::unifiedInput2Val(value.toString(),&a);
        if(a)
        {
            Offset->replace(index.row(),possibleEntry);
        }
        return a;
    }
    }
}

bool WatchModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
//    qDebug("The index of ("+index.row() + "," + index.column()+") is pointing at (" + Offset->indexOf(index.row())+","+index.column());
//     modeler* that = (modeler*)this;


    const QModelIndex offsetIndex = QAbstractItemModel::createIndex((int)(Offset->at(index.row())),index.column(),index.internalPointer());
    qDebug(QString().setNum(index.row()).toLocal8Bit());

    qDebug(QString().setNum(offsetIndex.row()).toLocal8Bit());
    bool a;
    if(offsetIndex.isValid())
    {
        switch(offsetIndex.column())
        {
            case Memory_Modulos::ADDRCOLUMN:return handleAddresColumn(index,value,role);

        }
    }


    return modeler::setData(offsetIndex,value,role);;
}
QModelIndex WatchModel::getOffsetIndex(const QModelIndex &index) const
{
    return QAbstractItemModel::createIndex((int)(Offset->at(index.row())),index.column(),index.internalPointer());
}



Qt::ItemFlags WatchModel::flags(const QModelIndex &index) const
{
    if((*threadRunning) && threadRunning!=Q_NULLPTR)return 0;

    const QModelIndex offsetIndex = getOffsetIndex(index);
    switch(offsetIndex.column())
    {
    case Memory_Modulos::ADDRCOLUMN:return modeler::flags(offsetIndex)|Qt::ItemIsDragEnabled|Qt::ItemIsDropEnabled|Qt::ItemIsEditable;
    }
    return (modeler::flags(index))|Qt::ItemIsDragEnabled|Qt::ItemIsDropEnabled;

    //    switch(index.column()){
    //    case BRCOLUMN   :return Qt::ItemIsUserCheckable+Qt::ItemIsEnabled;
    //    case ADDRCOLUMN :return Qt::NoItemFlags;
    //    case VALUCOLUMN :return Qt::ItemIsEditable;
    //    }

}
