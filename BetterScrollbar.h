/* Yeah, this is a mostly copy of highlightscrollbar that QtCreator uses.
 *
 * Since they don't have this in Qt on its own and porting it will be unbelievably
 * painful, I have decided to rewrite it
 *
 *
 */


#ifndef BETTERSCROLLBAR_H
#define BETTERSCROLLBAR_H

#include <QScrollBar>
#include "Utility.h"
class BetterScrollBar: public QScrollBar
{
    Q_OBJECT
public:
    BetterScrollBar(QWidget *parent = 0);


    void paintEvent(QPaintEvent * qpe);
};

#endif // BETTERSCROLLBAR_H
