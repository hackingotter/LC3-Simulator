#ifndef UNDOSTACKVIEW_H
#define UNDOSTACKVIEW_H

#include <QObject>
#include <QWidget>
#include <QUndoView>
#include <HistoryHandler.h>

class UndoStackView : public QUndoView
{
    Q_OBJECT
    QWidget* Jerry;
    HistoryHandler* history;

public:

    void pressed(const QModelIndex &index);

    UndoStackView(HistoryHandler* histo);

public slots:
    void test(QModelIndex);
};

#endif // UNDOSTACKVIEW_H
