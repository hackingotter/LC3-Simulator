#include "BetterScrollbar.h"
#include <QScrollBar>
#include <QPainter>
#include <QMap>
#include <QList>

#include <QStyleOptionSlider>



BetterScrollBar::BetterScrollBar(QWidget* parent):QScrollBar(parent)
{

}

void BetterScrollBar::paintEvent(QPaintEvent * qpe)
{
    QScrollBar::paintEvent(qpe);//this handles all the stuff that I don't want to do.
    const int height = size().height();
    const int width  = size().width();

    /**
     * @brief THIS NEEDS TO BE FIXED I DON'T KNOW HOW TO FIND THE ARROW'S SIZE
     */

    const int offsetMin = 16;
    /**
     * @brief THIS NEEDS TO BE FIXED I DON'T KNOW HOW TO FIND THE ARROW'S SIZE
     */
    const int offsetMax = height-16;

    const int contentHeight = offsetMax-(offsetMin);

//    QStyleOptionSlider sty;
//    initStyleOption(&sty);
//    BATHTIME(QString().setNum(style()->subControlRect(QStyle::CC_ScrollBar,&sty,QStyle::SC_ScrollBarGroove,this).height()));


//    BATHTIME(QString("Height %1").arg(height))
//            BATHTIME(QString("Width %1").arg(width))
//            BATHTIME(QString("Minimum %1").arg(minimum()))


//            BATHTIME(QString("Height %i").arg(height))
//            BATHTIME(QString("Height %i").arg(height))
//            BATHTIME(QString("Height %i").arg(height))

    QPainter leo(this);

    leo.setPen(Qt::black);




    int den = 4;
    leo.setPen(Qt::red);
//    for(int i = 0;i<=den;i++)  leo.drawLine(0,i*(height/den),width/2,i*(height/den));
    leo.setPen(Qt::blue);
    den = 150;
//    for(int i = 0;i<=den;i++)  leo.drawLine(width/2,i*(contentHeight/den)+offsetMin,width,i*(contentHeight/den)+offsetMin);

    leo.end();

}

