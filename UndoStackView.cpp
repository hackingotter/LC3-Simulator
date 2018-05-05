#include "UndoStackView.h"

UndoStackView::UndoStackView(HistoryHandler *histo)
{
    qDebug("Successful initialization");
    setStack(histo);


    history = histo;
    connect(this,&QAbstractItemView::pressed,this,&UndoStackView::test);
}
void UndoStackView::pressed(const QModelIndex &index)
{
    qDebug("Pressed");
    history->mask();
    this->QUndoView::pressed(index);
    history->unmask();
}
void UndoStackView::test(QModelIndex)
{
history->mask();
}
