#include "UndoStackView.h"
#include "QPainter"
#include "QLayout"


UndoStackView::UndoStackView(HistoryHandler* solo, QWidget *parent):QUndoView(solo,parent)
{

}

