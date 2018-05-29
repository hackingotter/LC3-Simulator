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

};

#endif // UNDOSTACKVIEW_H
