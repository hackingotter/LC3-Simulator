#include "WatchModel.h"
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

}
QVariant WatchModel::data(const QModelIndex &index, int role) const
{

    QModelIndex offsetIndex = createIndex((int)(Offset->at(index.row())),index.column(),index.internalPointer());
    return modeler::data(offsetIndex,role);
}

bool WatchModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
//    qDebug("The index of ("+index.row() + "," + index.column()+") is pointing at (" + Offset->indexOf(index.row())+","+index.column());
//     modeler* that = (modeler*)this;

    const QModelIndex offsetIndex = QAbstractItemModel::createIndex((int)(Offset->at(index.row())),index.column(),index.internalPointer());
    qDebug(QString().setNum(index.row()).toLocal8Bit());

    qDebug(QString().setNum(offsetIndex.row()).toLocal8Bit());

    return modeler::setData(offsetIndex,value,role);;
}
