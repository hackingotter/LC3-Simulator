#include "hope.h"
#include "mainwindow.h"
#include <QRgb>
#include <QImage>

#define testinter 3

#define color(in)((in/31.0)*255)


#define cursordraw(width,height,painter,dif,offsetx,offsety)\
painter.drawLine(offsetx+1      ,offsety+1      ,offsetx+dif        ,offsety+dif          );\
painter.drawLine(offsetx+width  ,offsety+0      ,offsetx+width-dif  ,offsety+dif          );\
painter.drawLine(offsetx+width  ,offsety+height ,offsetx+width-dif  ,offsety+height-dif   );\
painter.drawLine(offsetx+0      ,offsety+height ,offsetx+dif         ,offsety+height-dif   );

#define ADDR2X(ADDR) ((ADDR - VIDEO_ADDR)%DISPLAY_WIDTH)
#define ADDR2Y(ADDR) ((ADDR - VIDEO_ADDR)/DISPLAY_HEIGHT)
#define ADDR2POINT(ADDR) (DISPLAY_SCALE*ADDR2X(ADDR)),(DISPLAY_SCALE*ADDR2Y(ADDR))
QPicture pi;

//QPainter p(&pi);
QRect rekt;
Hope::Hope(QWidget *parent):QLabel(parent,0)
{
    qDebug("Setting up screen");

//    model = guardin->get
    mag = new QImage(DISPLAY_WIDTH*DISPLAY_SCALE,DISPLAY_HEIGHT*DISPLAY_SCALE,QImage::Format_RGB32);
    setMouseTracking(true);
    QPainter p(&pi);
    p.setRenderHint(QPainter::Antialiasing,true);
    p.setPen(Qt::black);
    for(int i = 0;i<124;i++)
    {
        for(int j=0;j<128;j++)
        {
            if(i%testinter ==0||j%testinter==0) p.drawLine(2*i,2*j,2*i+2,2*j+2);
            p.drawPoint(2*i,2*j);
            if((i==0||i==123)&&(j==0||j==127))
            {
                p.setPen(Qt::black);
                p.drawPoint(2*i,2*j);
            }
        }
    }
//    p.end();
    setPicture(pi);
    rekt = pi.boundingRect();
    qDebug("Done finding picture size");
            QString* sizer = new QString();


    QPicture picture = QPicture();
//    QSize cursorsize = new QCursor(Qt::CrossCursor)->bitmap()->size();
//    QPixmap* pix = new QPixmap(cursorsize.width(),cursorsize.height());
    QString* str = new QString();
//            str->setNum(cursorsize.height());
            str->append('\0');

            qInfo(str->toLatin1());
//    QPainter* paint();
    Hope::customCursor(0,0);
    qDebug("hey");
    qDebug(QString(styleSheet()).toLatin1().data());

}


void Hope::mouseMoveEvent(QMouseEvent *ev)
{


//    BATHTIME("The mouse has moved!")
    QPainter p(&pi);
    pi.setBoundingRect(rekt);
    p.setRenderHint(QPainter::Antialiasing,true);

    int x = ev->x()/2;
    int y = ev->y()/2;

    Hope::customCursor(ev->x(),ev->y());
    QString xstr = QString("");
    xstr.setNum(x,10);
    QString ystr = QString("");
    ystr.setNum(y,10);
    QString astr = QString("");
    astr.setNum(x+y*DISPLAY_WIDTH + VIDEO_ADDR,16);
    QString ostr = QString("");
    ostr.append(xstr);
    ostr.append(",");
    ostr.append(ystr);
    ostr.append(",");
    ostr.append(astr);
    xstr.append('\0');
    emit Hope::mouseMoved(ostr);
//    BATHTIME(ostr.toLatin1());
    DRAWPOINT(x,y,p)
    p.end();
//    setPicture(pi);


    show();

}
void Hope::mousePressEvent(QMouseEvent *ev)
{
    update();

}
QColor Hope::translater(int in)
{
    /*
     *Since the colors are 5 bits, they are compared with five 1's then recorded
     * three times, getting the r, g, and b , which are then changed from a
     * range of 31 to 255, and composed into a color
     */
    double b = in&COLORSLICE;in>>= 5;
    double g = in&COLORSLICE;in>>= 5;
    double r = in&COLORSLICE;
    return QColor(color(r),color(g),color(b),255);
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
    for(mem_addr_t i = VIDEO_ADDR;i<MEMSIZE;i++)
    {
        Computer::getDefault()->setMemValue(i,val);//TODO replace with setMemValuesBlock when done
    }
    update();
}
void Hope::update()
{

//    QPainter qpp(&pi);
    for(mem_addr_t i = VIDEO_ADDR;i<MEMSIZE;i++)
    {
//        mag.setPen(translater(getMemValue(i)));
//        DRAWPOINT(ADDR2X(i),ADDR2Y(i),qpp);

        setPoint(ADDR2X(i),ADDR2Y(i),translater(Computer::getDefault()->getMemValue(i)).rgb(),mag);
    }
    setPixmap(QPixmap::fromImage(*mag));
//    qpp.end();
//    setPicture(pi);

}
void Hope::update(mem_addr_t addr)
{
    qDebug("Updating point");

    QPainter qpp(&pi);
//    mag.setPixel(ADDR2X(addr),ADDR2Y(addr),translater(getMemValue(addr)).rgb());

    setPoint(ADDR2X(addr),ADDR2Y(addr),translater(Computer::getDefault()->getMemValue(addr)).rgb(),mag);
//    qpp.end();
    QPixmap pix = QPixmap::fromImage(*mag);
    qDebug(QString().setNum(pix.height()).toLocal8Bit());
    setPixmap(pix);
    qDebug("done updating");
}
void Hope::setPoint(int x, int y,QRgb rgb, QImage*image)
{
    if(x<0||y<0) return;
    {
        for(int i = 0;i<DISPLAY_SCALE;i++)
        {
            for(int j = 0;j<DISPLAY_SCALE;j++)
            {
                image->setPixel((x*DISPLAY_SCALE)+i     ,(y*DISPLAY_SCALE)+j,rgb);

            }
        }

    }
}

