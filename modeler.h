#ifndef MODELER_H
#define MODELER_H

#include <QStandardItemModel>
#include <QUndoStack>
#include "DoUndo.h"
#include "Memory.h"

#define NUMBER_OF_COLUMNS 6



namespace Memory_Modulos {
    enum modelColumn{
    BRCOLUMN = 0,
    ADDRCOLUMN = 1,
    NAMECOLUMN = 2,
    VALUCOLUMN = 3,
    MNEMCOLUMN = 4,
    COMMCOLUMN = 5
    };
}

class modeler : public QStandardItemModel
{
    Q_OBJECT

public:
    QUndoStack* noTry;
    QObject* paternal_Figure;
    bool* threadRunning;
    explicit modeler(QObject *parent = 0,bool* access= Q_NULLPTR);

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role = Qt::EditRole) override;

    // Basic functionality:
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    void setQUndoStack(QUndoStack*);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;
    bool setValue(int row,int value,int role);
    QObject* paternal();
    void update();

    QString mnemonicGen(mem_addr_t addr)const;
signals:
    void dataChan(mem_addr_t);
    void valueChanged(mem_addr_t,val_t);
    void nameChanged(mem_addr_t,label_t*);
    void breakChanged(mem_addr_t,breakpoint_t*);
    void commentChanged(mem_addr_t,char *);
    void requestMemValueChange(mem_addr_t,val_t,int);
    void change();
private:
};

#endif // MODELER_H
