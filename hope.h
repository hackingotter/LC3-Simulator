 #ifndef HOPE_H
#define HOPE_H

#include <QLabel>
#include <QCursor>
#include <QBitmap>
#include <QStyle>
#include <QWidget>
#include <QPicture>
#include <QPainter>
#include <QMouseEvent>
#include <QImage>
#include "GlobalTypes.h"

#define COLORSLICE 0x1F

#define CLEAR_SCREEN_COLOR (0)

#define DRAWPOINT(X,Y,PAINTER)\
    PAINTER.drawPoint(2*(X),2*(Y));\
    PAINTER.drawPoint(2*(X)+1,2*(Y));\
    PAINTER.drawPoint(2*(X),2*(Y)+1);\
    PAINTER.drawPoint(2*(X)+1,2*(Y)+1);


#define CURSORSIZE 8
#define PRONGLENGTH 2

class Hope: public QLabel
{
    Q_OBJECT
public:
    QImage*mag;
    explicit Hope(QWidget *parent = 0);
//    Hope(QWidget *parent,QStandardItemModel* model);

    /** Handles the clicking
     * \param ev the event
     */
    void mousePressEvent(QMouseEvent *ev);
    void Setup();
    QColor translater(int in);
//
    void mouseMoveEvent(QMouseEvent *ev);

    void fillScreen(val_t val);
    void setPoint(int x, int y, QRgb rgb, QImage*image);
public slots:
    void clearScreen();
    void update();
    void update(mem_addr_t addr);
signals:
    void mouseMoved(QString);
private:
    void customCursor(int x, int y);

};

#endif // HOPE_H
