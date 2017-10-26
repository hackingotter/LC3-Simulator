#ifndef MODELDELEGATE_H
#define MODELDELEGATE_H
#include "Utility.h"
#include <QAbstractItemDelegate>
class ModelDelegate : public QAbstractItemDelegate
{
public:

    explicit ModelDelegate(QObject* parent = 0);
};

#endif // MODELDELEGATE_H
