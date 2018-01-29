#ifndef SCROLLBARHANDLER_H
#define SCROLLBARHANDLER_H

#include <QObject>
#include "BetterScrollbar.h"
#include <QLinkedList>
#include "Utility.h"
#include "computer.h"
class ScrollBarHandler : public QObject
{
    Q_OBJECT

    QLinkedList<HighlightScrollBar*> bars;
public:
    explicit ScrollBarHandler(QObject *parent = nullptr);
    void addScrollBar(HighlightScrollBar* bar);
    void update();
    HighlightScrollBar *generateBar(QWidget *parent=0);
    void removeScrollBar(HighlightScrollBar *Bar);
signals:

public slots:
};

#endif // SCROLLBARHANDLER_H
