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

    val_t * stackFrameColors;
public:

    explicit StackModeler(QObject *parent = 0,bool* access= Q_NULLPTR);
    QVariant data(const QModelIndex &index,int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QBrush stackFramePainter(mem_addr_t addr)const;
    val_t colorHash(val_t val);
public slots:
    void flip();
    void setTopBig(bool ghost);
    void stackFrameListener(mem_addr_t addr);
private:
    bool * bigTop = new bool(true);
};

#endif // STACKMODELER_H
