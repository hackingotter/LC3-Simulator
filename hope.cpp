#include "hope.h"
#include "mainwindow.h"
#include <QRgb>
#include "computer.h"
#include <QImage>

#define testinter 3

#define color(in)((in/31.0)*255)


#define cursordraw(width,height,painter,dif,offsetx,offsety)\
painter.drawLine(offsetx+1      ,offsety+1      ,offsetx+dif        ,offsety+dif          );\
painter.drawLine(offsetx+width  ,offsety+0      ,offsetx+width-dif  ,offsety+dif          );\
painter.drawLine(offsetx+width  ,offsety+height ,offsetx+width-dif  ,offsety+height-dif   );\
painter.drawLine(offsetx+0      ,offsety+height ,offsetx+dif         ,offsety+height-dif   );

#define ADDR2X(ADDR) DISPLAY_SCALE*((ADDR - VIDEO_ADDR)%DISPLAY_WIDTH)
#define ADDR2Y(ADDR) DISPLAY_SCALE*((ADDR - VIDEO_ADDR - ADDR2X(ADDR)/DISPLAY_SCALE)/DISPLAY_HEIGHT)
#define ADDR2POINT(ADDR) (ADDR2X(ADDR)),(ADDR2Y(ADDR))
QPicture pi;

//QPainter p(&pi);
QRect rekt;
Hope::Hope(QWidget *parent):QLabel(parent,0)
{
    qDebug("Setting up screen");
//    QPicture pi = new QPicture();

    mag = new QImage(DISPLAY_WIDTH*DISPLAY_SCALE,DISPLAY_HEIGHT*DISPLAY_SCALE,QImage::Format_RGB32);
    setPixmap(QPixmap::fromImage(*mag));
    Hope::customCursor(0,0);
    setMouseTracking(true);
}
void Hope::left()
{
    QCursor c = cursor();
    c.setPos(cursor().pos().x()-1,cursor().pos().y());
    setCursor(c);
}
void Hope::right()
{
    QCursor c = cursor();
    c.setPos(cursor().pos().x()+1,cursor().pos().y());
    setCursor(c);
}
void Hope::up()
{
    QCursor c = cursor();
    c.setPos(cursor().pos().x(),cursor().pos().y()+1);
    setCursor(c);
}
void Hope::down()
{
    QCursor c = cursor();
    c.setPos(cursor().pos().x(),cursor().pos().y()-1);
    setCursor(c);
}

void Hope::mouseMoveEvent(QMouseEvent *ev)
{


//    BATHTIME("The mouse has moved!")
    QPainter p(&pi);
    pi.setBoundingRect(rekt);
    p.setRenderHint(QPainter::Antialiasing,true);

    int x = ev->x()/DISPLAY_SCALE;
    int y = ev->y()/DISPLAY_SCALE;

    Hope::customCursor(ev->x(),ev->y());
    QString xstr = QString("").setNum(x,10).rightJustified(3,' ');
    QString ystr = QString("").setNum(y,10).rightJustified(3,' ');
    QString astr = QString("").setNum(x+y*DISPLAY_WIDTH + VIDEO_ADDR,16);
    QString ostr = getHexString(Computer::getDefault()->getMemValue(x+y*DISPLAY_WIDTH + VIDEO_ADDR));
    ostr.append(" ("+xstr);
    ostr.append(",");
    ostr.append(ystr+")");
    ostr.append(" @");
    ostr.append(astr);
    xstr.append('\0');
    emit Hope::mouseMoved("Current Mouse Position:" +ostr);
//    BATHTIME(ostr.toLatin1());
    DRAWPOINT(x,y,p)
    p.end();
//    setPicture(pi);


    show();

}
void Hope::mousePressEvent(QMouseEvent *)
{
    update();

}

void Hope::customCursor(int x,int y)
{
    /**
     * I want the cursor to look as if it is on half resolution
     * so the following code does just that
     *
     * First, an image for the cursor is offset by the proper amount so
     * that it looks as if it wasn't moving, then the cursor is given
     * that image
     */




    QPixmap custom = QPixmap(8,8);
    custom.fill(Qt::transparent);
    QPainter j(&custom);
    j.setPen(Qt::white);
    cursordraw(8,8,j,2,-x%2,-y%2);
    j.end();
    setCursor(QCursor(custom,-3,-3));
}
void Hope::clearScreen()
{
    fillScreen(CLEAR_SCREEN_COLOR);
}
void Hope::fillScreen(val_t val)
{
    Computer::getDefault()->Undos->beginMacro("Filling screen with "+ getHexString(val));
    MASK

    Computer::getDefault()->fillBlock(VIDEO_ADDR,0xFE00,val);
    UNMASK
    Computer::getDefault()->Undos->endMacro();
}
void Hope::update()
{

    IFCANUPDATE(
    qDebug("Hope is updating");
//    QPainter qpp(&pi);
    for(mem_addr_t i = VIDEO_ADDR;i<0xFE00;i++)
    {
        int x = (i - VIDEO_ADDR) % DISPLAY_WIDTH;
        int y = (i - VIDEO_ADDR - x) / DISPLAY_WIDTH;

//            mag->setPixel(x+1, y+1, translater(i).rgb());
//        mag.setPen(translater(getMemValue(i)));
//        DRAWPOINT(ADDR2X(i),ADDR2Y(i),qpp);

        setPoint(x*DISPLAY_SCALE,y*DISPLAY_SCALE,Utility::translater(Computer::getDefault()->getMemValue(i)).rgb(),mag);
    }
    setPixmap(QPixmap::fromImage(*mag));
    qDebug((QString().setNum(pixmap()->width()) + QString().setNum(pixmap()->height())).toLocal8Bit());
//    qpp.end();
//    setPicture(pi);
)
}
void Hope::update(mem_addr_t addr)
{

    if(VIDEO_ADDR<= addr && addr<0xfe00)
    {
//    QPainter qpp(&pi);
//    mag.setPixel(ADDR2X(addr),ADDR2Y(addr),translater(getMemValue(addr)).rgb());
    int x = (addr - VIDEO_ADDR) % DISPLAY_WIDTH;
    int y = (addr - VIDEO_ADDR - x) / DISPLAY_WIDTH;

//        mag->setPixel(x+1, y+1, translater(1).rgb());
    setPoint(x,y,Utility::translater(Computer::getDefault()->getMemValue(addr)).rgb(),mag);
//    qpp.end();
    QPixmap pix = QPixmap::fromImage(*mag);

    qDebug(QString().setNum(pix.height()).toLocal8Bit());
    setPixmap(pix);
    }
    qDebug("done updating");

}
void Hope::setPoint(int x, int y,QRgb rgb, QImage*image)
{
    image->setPixel(x  ,y  ,rgb);
    image->setPixel(x  ,y+1,rgb);
    image->setPixel(x+1,y+1,rgb);
    image->setPixel(x+1,y  ,rgb);
/*    if(x<0||y<0) return;
    {
        for(int i = 0;i<DISPLAY_SCALE;i++)
        {
            for(int j = 0;j<DISPLAY_SCALE;j++)
            {
                image->setPixel((x*DISPLAY_SCALE)+i     ,(y*DISPLAY_SCALE)+j,rgb);

            }
        }

    }
*/
}

