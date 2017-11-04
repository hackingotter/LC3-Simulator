#include "KBRDModel.h"
#include "computer.h"
#include <QStandardItemModel>
KBRDModel::KBRDModel(QObject *parent): QStandardItemModel(parent)
{
    setColumnCount(1);
    setRowCount(1);

}
QVariant KBRDModel::data(const QModelIndex &index, int role) const
{
    return Computer::getDefault()->getMemValue(KBDR);

}
