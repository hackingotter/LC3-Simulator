#ifndef INOUTSET_H
#define INOUTSET_H

#include <QWidget>
#include <QTableView>
#include <QPushButton>
#include <QLabel>

class InOutSet : public QWidget
{
    Q_OBJECT
public:
    InOutSet(QWidget *parent = 0);

    void update();
    void keyPressEvent(QKeyEvent *event);

private:
    QPushButton* Clear;
    QPushButton* Take;
    QLabel* textDisplay;



};

#endif // INOUTSET_H
