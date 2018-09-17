#ifndef REGISTERMODEL_H
#define REGISTERMODEL_H

#include <QObject>
#include <QTableView>
#include <QUndoStack>
#include <QStandardItemModel>

#include "Bridge.h"


#define REGCOLUMNCOUNT 3
typedef enum reg_column{
    reg_color_column = 0,
    reg_name_column  = 1,
    reg_value_column = 2

} reg_column;



#define REGNAMECOlUMN  1
#define REGVALUECOLUMN 2

#define MPR_SLOT 10


#define PSR_HANDLER(INPUT,CHECK,VALUE) if(INPUT==CHECK) return VALUE;

class RegisterModel : public QStandardItemModel
{
    Q_OBJECT
public:


    QList<QColor> *regColors;
    QList<bool> *regColorsCheck;
    explicit RegisterModel(QObject* parent = 0,bool* excersize=Q_NULLPTR);

    QVariant headerData(int section,Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role = Qt::EditRole) override;

    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;

    QModelIndex parent(const QModelIndex &index) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index,int role =  Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant&value,int role) override;

    Qt::ItemFlags flags(const QModelIndex&index) const override;

    cond_t handle_CC_RegisterView_Input(QString in);

    QString static regNameColumnHelper(int row) ;

    QList<QColor>* getRegColors();

    void update();


    static QString regValueHelper(reg_t reggy);
signals:
    void requestRegisterEdit(reg_t,val_t,int);
    void regEdit(reg_t,val_t) const;
    void psrEdit(cond_t);
    void colorEdit(int row,QColor fresh);
    void change();
    void requestUpdate();
public slots:
//    void setActive();

private:
    bool* threadRunning;
};

#endif // REGISTERMODEL_H
