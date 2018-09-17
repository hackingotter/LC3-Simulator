#ifndef INOUTSET_H
#define INOUTSET_H

#include <QWidget>
#include <QTableView>
#include <QPushButton>
#include <QLabel>
#include "computer.h"

class InOutSet : public QWidget
{
    Q_OBJECT
public:
    InOutSet(QWidget *parent = 0);

    void update();
    void keyPressEvent(QKeyEvent *event);
public slots:
    void kick();
    void pushChar(val_t);
    void popChar();
    void clearText();

private:
    QLabel* InputMonitor;
    QLabel* OutputMonitor;
    QLabel* textDisplay;



};

#endif // INOUTSET_H
