#include "Utility.h"
#include "opcodes.h"


#define color(in)((in/31.0)*255)
namespace Utility {


QColor translater(val_t in)
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
QString charToQString(char ch)
{
    if(ch > 0x7E)
    {
        return QString(ch);
    }
    switch(ch)
    {
    case '\0':
        return "\\0";
    case '\a':
        return "\\a";
    case '\b':
        return "\\b";
    case '\t':
        return "\\t";
    case '\n':
        return "\\n";
    case '\v':
        return "\\v";
    case '\f':
        return "\\f";
    case '\r':
        return "\\r";
    case '\e':
        return "\\e";
    }
    if(0x20 <= ch && ch <= 0x7E)
    {
        return QString().append(QChar(ch));
    }
    return "Undeclared Char";

}
int QSTRING2INTBASE(QString source,int suggestedBase)
{
    qDebug("Looks like we are changing a QString to a Int ");
    bool noError = 0;
    //First things first, let's see what Qt gives us
    int qtAttempt = source.toInt(&noError,suggestedBase);
    if(noError)
    {
        //If it worked, use it
        return qtAttempt;
    }
    //If not, let's try changing
    QString a = "Hello";



}

void systemInfoDebug()
{

    qDebug(QSysInfo::currentCpuArchitecture().toLocal8Bit());

    qDebug(QString("The computer: ").append(QSysInfo::prettyProductName()).toLocal8Bit());
}

#define QVARIANT2VAL_T(QVARIANT) (static_cast<val_t>(QVARIANT.toString().toInt(nullptr,16)))

int qColor2Int(QColor color)
{
    int r,g,b,a;
    color.getRgb(&r,&g,&b,&a);
    return ((((r<<8)+g)<<8)+b)<<8 + a;
}

QColor int2QColor(int color)
{
    int a = color&0xFF;
    int b = (color>>8)&0xFF;
    int g = (color>>16)&0xFF;
    int r = (color>>24)&0xFF;
    return  QColor(r,g,b,a);
}
val_t unifiedInput2Val(QString input,bool* ok = Q_NULLPTR)
{
    *ok = true;//Clean the input.
    qDebug(QString().setNum(*ok).toLocal8Bit());
    if(input == "")
    {
        *ok = false;
        return 0;
    }
    qDebug(input.toLocal8Bit());
    val_t out;
    if(input.at(0) == '#')
    {
        input.remove(0,1);
        out = input.toInt(ok,10);
        if(*ok)return out;
    }
    if(input.at(0)=='0')//pre prefix 0x, 0b, etc
    {
        if(input.length()==1)
        {
            *ok = true;
            return 0;
            //When I wrote this code, I forgot something.
            //I think this edge case has been missed
            //for months now.  Fixed 1/4/2018
            //-Melberg
        }

        input.remove(0,1);//get rid it
        qDebug(input.toLocal8Bit());
    }
    if(input.at(0) == 'b')
    {
        input.remove(0,1);
        out = input.toInt(ok,2);
        return (*ok)?out:0;
    } else
        if(input.at(0) == 'o')
        {
            input.remove(0,1);
            out = input.toInt(ok,8);
            return (*ok)?out:0;
        } else
            if(input.at(0) == 'x')
            {
                input.remove(0,1);
                qDebug(input.toLocal8Bit());
                out = input.toInt(ok,16);
                return (*ok)?out:0;
            }
            else
            {
                out = input.toInt(ok,10);//this is the default
                return (*ok)?out:0;
            }
    *ok = false;
    return 0;
}

void Utilit::setup()
{

}
mem_addr_t Utilit::specialSelectStart = 1;
mem_addr_t Utilit::specialSelectEnd = 0;





}
