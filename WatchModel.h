#ifndef WATCHMODEL_H
#define WATCHMODEL_H
#include "modeler.h"
#include <QObject>

class WatchModel : public modeler
{
    Q_OBJECT
public:
    WatchModel(QObject* parent = 0,bool* access = Q_NULLPTR);
    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) ;
private:
    QModelIndex getOffsetIndex(const QModelIndex &index) const;
    QVector<val_t>* Offset;
    Qt::ItemFlags flags(const QModelIndex &index) const;

    bool handleAddresColumn(const QModelIndex &index, const QVariant &value, int role);
};


#endif // WATCHMODEL_H
