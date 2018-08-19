
#include "UndoStackMasker.h"
#include "QPushButton"
#include "QMouseEvent"
#include "qevent.h"
#include "QLayout"
#include "QApplication"
#include "QGraphicsScene"
#include "QScrollBar"
#include "QPainter"
#include "computer.h"
#include "QPalette"
//#include <QTest>
//#include "qtest
UndoStackMasker::UndoStackMasker(HistoryHandler* historian,QWidget *parent) : QWidget(parent)
{
    Historian = historian;
    QSV = new UndoStackView(Historian,this);
    QSV->setVerticalScrollBar(new QScrollBar(Qt::Vertical));
    QSV->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOn);
    Middleman = new QWidget(this);
    Middleman->setVisible(true);
    QSV->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    connect(Historian,&HistoryHandler::indexChanged,this,&UndoStackMasker::updateSize);
    connect(Historian,&HistoryHandler::indexChanged,this,&UndoStackMasker::signalFlare);
    connect(Historian,&HistoryHandler::indexChanged,this,&UndoStackMasker::finishedDoing);
    connect(QSV,&UndoStackView::pressed,this,&UndoStackMasker::pres);

//    connect(QSV,&QUndoView::,this,&UndoStackMasker::startDoing);
//    QSV->
}
void UndoStackMasker::pres(const QModelIndex &mold)
{
//    if(mold)
//    {
    qDebug(QString().setNum(mold.row()).toLocal8Bit());
//    }
//    else
//    {
//        qDebug("No index");
//    }
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
    out->setWidth(this->width());
    out->setHeight(this->height());

    const QSize conout =  QSize(out->width(),out->height());
    qDebug(QString().setNum(conout.height()).toLocal8Bit());
    qDebug(QString().setNum(conout.width()).toLocal8Bit());
    delete(out);
    return conout;
}


void UndoStackMasker::mousePressEvent(QMouseEvent *event)
{
    MASK
    QMouseEvent pressEvent(QEvent::MouseButtonPress,event->pos(),Qt::LeftButton,Qt::LeftButton,Qt::NoModifier);
    QApplication::sendEvent(QSV->viewport(), &pressEvent);
    UNMASK
}

void UndoStackMasker::startDoing()
{
    MASK
    qDebug("HHHHH");
}
void UndoStackMasker::finishedDoing()
{
    UNMASK
    IFNOMASK(Computer::getDefault()->update();)
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
