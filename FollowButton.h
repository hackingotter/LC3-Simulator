#ifndef FOLLOWBUTTON_H
#define FOLLOWBUTTON_H
#include "QPushButton"
#include "QWidget"
/*
 * All I want is to be able to listen for a double click, something which I
 * can't do without making a new class where that information is public.
 */
class FollowButton: public QPushButton
{
    Q_OBJECT
public:
    void mouseDoubleClickEvent(QMouseEvent *);
    FollowButton(QString text);
signals:
    void doubleClicked();
};
#endif // FOLLOWBUTTON_H
