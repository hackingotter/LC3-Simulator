#ifndef MEMWINDOW_H
#define MEMWINDOW_H

#include <QWidget>
#include "MemTable.h"
#include "modeler.h"
#include "QPushButton"
#include "QLineEdit"
#include "QLabel"
#include "computer.h"


#define CLEARONGOTO (true)

#define CLEAR(INPUT) if(CLEARONGOTO)INPUT->clear();

class MemWindow : public QWidget
{
    Q_OBJECT
    MemTable* View;
    QLabel* Label;
    QLineEdit* Input;
    QPushButton* GotoButton;
    QPushButton* PCButton;

public:
    explicit MemWindow(modeler* model, QWidget *parent = nullptr);

signals:

public slots:
private slots:
    void handlePCPress();
    void handleGotoPress();
};

#endif // MEMWINDOW_H
