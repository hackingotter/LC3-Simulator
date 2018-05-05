#include "UndoStackMasker.h"
#include "QPushButton"
#include "QMouseEvent"
#include "qevent.h"
#include "QApplication"
#include "QGraphicsScene"
#include "QScrollBar"
#include "QPalette"
//#include <QTest>
//#include "qtest
UndoStackMasker::UndoStackMasker(HistoryHandler* historian,QWidget *parent) : QWidget(parent)
{
    Historian = historian;
    QSV = new QUndoView(Historian,this);
    QSV->setVerticalScrollBar(new QScrollBar(Qt::Vertical));
    QSV->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOn);
    Middleman = new QWidget(this);
    Middleman->setVisible(true);
    connect(Historian,&HistoryHandler::indexChanged,this,&UndoStackMasker::updateSize);
    connect(Historian,&HistoryHandler::indexChanged,this,&UndoStackMasker::signalFlare);

}
void UndoStackMasker::updateSize()
{
    Middleman->resize(constructSize());
}
void UndoStackMasker::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);



}

const QSize UndoStackMasker::constructSize()
{
    QSize* out = new QSize();
    qDebug("Phase 2" + QString().setNum(QSV->viewport()->width()).toLocal8Bit());
    out->setWidth(QSV->viewport()->width());
    out->setHeight(QSV->verticalScrollBar()->size().height());

    const QSize conout =  QSize(out->width(),out->height());
    qDebug(QString().setNum(conout.height()).toLocal8Bit());
    qDebug(QString().setNum(conout.width()).toLocal8Bit());
    return conout;
}


void UndoStackMasker::mousePressEvent(QMouseEvent *event)
{
    qDebug("IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII");
    Historian->mask();
    QMouseEvent pressEvent(QEvent::MouseButtonPress,event->pos(),Qt::LeftButton,Qt::LeftButton,Qt::NoModifier);
    QApplication::sendEvent(QSV->viewport(), &pressEvent);
}

void UndoStackMasker::mouseReleaseEvent(QMouseEvent *event)
{
    QMouseEvent releaseEvent(QEvent::MouseButtonRelease,event->pos(),Qt::LeftButton,Qt::LeftButton,Qt::NoModifier);
    QApplication::sendEvent(QSV->viewport(), &releaseEvent);
}
void UndoStackMasker::mouseMoveEvent(QMouseEvent *event)
{
    //    QApplication::sendEvent(QSV->viewport(),event);
}
