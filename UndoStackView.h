#ifndef UNDOSTACKVIEW_H
#define UNDOSTACKVIEW_H

#include <QObject>
#include <QWidget>
#include <QUndoView>
#include <QLayoutItem>
#include <HistoryHandler.h>
class UndoStackView : public QUndoView
{
public:
    UndoStackView(HistoryHandler *solo, QWidget* parent =nullptr);


private:
    void paintEvent(QPaintEvent *e);
    void paintLayout(QPainter *painter, QLayoutItem *item);
};

#endif // UNDOSTACKVIEW_H
