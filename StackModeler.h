#ifndef STACKMODELER_H
#define STACKMODELER_H
#include "TrainFlagged.h"
#include <QModelIndex>
#include <QStandardItemModel>
#include "modeler.h"
#include <QObject>


namespace Stack_Modulos {
enum StackColumn{
    ADDRCOLUMN   = 0,
    OFFSETCOLUMN = 1,
    NAMECOLUMN   = 2,
    VALUECOLUMN  = 3
};
}
class StackModeler :public modeler
{
    Q_OBJECT
public:

    explicit StackModeler(QObject *parent = 0,bool* access= Q_NULLPTR);
    QVariant data(const QModelIndex &index,int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
public slots:
    void flip();
    void setTopBig(bool ghost);
private:
    bool * bigTop = new bool(true);
};

#endif // STACKMODELER_H
