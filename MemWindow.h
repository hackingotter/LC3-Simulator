#ifndef MEMWINDOW_H
#define MEMWINDOW_H

#include <QWidget>
#include "MemTable.h"
#include "modeler.h"
#include "QPushButton"
#include "QLineEdit"
#include "QLabel"
#include "BetterScrollbar.h"
#include "computer.h"
#include "FollowButton.h"
#include <map>

#define CLEARONGOTO (true)

#define CLEAR(INPUT) if(CLEARONGOTO)INPUT->clear();

class MemWindow : public QWidget
{

    Q_OBJECT
    bool flipped;
    std::map<QString, uint16_t> labelDict;
    MemTable* View;
    HighlightScrollBar* Scroll;
    QLabel* Label;
    QLineEdit* Input;
    QPushButton* GotoButton;
    FollowButton* SpecialButton;
    reg_t SpecialReg=PC;

    /*
     * SR-Follow Feature
     *
     * There is a special action that the user can evoke from the program.
     *
     * By double clicking the PCButton, the table goes into a 'follow mode'
     * where the view will track the line of code pointed to by the PC reg,
     * allowing for easy view of how the program is going and sparing them
     * the tedium of having to hit the aforementioned button whenever the
     * program counter moves too far.
     *
     */
    bool followmode=false;

    int getScrollOffset(val_t row, bool *ok);
    void setupInput();
public:
    explicit MemWindow(modeler* model, HighlightScrollBar *scroll, QWidget *parent = nullptr,QString* buttonName = new QString("PC"));
    MemTable* getMemView();
    void setFlipped(bool upIsDown);
signals:

public slots:
    void kick();

    void handleTracking();
    void setSpecialReg(reg_t reg);

private slots:
    void handleSRPress();
    void handleGotoPress();
};

#endif // MEMWINDOW_H
