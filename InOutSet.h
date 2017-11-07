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
    void clearText();
    void update();
    void keyPressEvent(QKeyEvent *event);
public slots:
    void kick();
    void pushChar(val_t);
    void popChar();


private:
    QPushButton* Clear;
    QPushButton* Take;
    QLabel* textDisplay;



};

#endif // INOUTSET_H
