#ifndef KBRDMODEL_H
#define KBRDMODEL_H
#include<QStandardItem>
#include <QObject>
class KBRDModel: public QStandardItemModel
{
    Q_OBJECT

public:
    explicit KBRDModel(QObject* parent=0);
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

};

#endif // KBRDMODEL_H
