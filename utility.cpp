#include "Utility.h"
#include "opcodes.h"

namespace Utility {






int QSTRING2INTBASE(QString source,int suggestedBase)
{
    qDebug("Looks like we are changing a QString to a Int ");
            bool* noError;
    //First things first, let's see what Qt gives us
    int qtAttempt = source.toInt(noError,suggestedBase);
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
val_t unifiedInput2Val(QString input)
{
    qDebug(input.toLocal8Bit());
    val_t out;
    bool* ok = static_cast<bool*>(malloc(sizeof(bool)));
    if(input.at(0) == '#')
    {
        input.remove(0,1);
        out = input.toInt(ok,10);
        if(*ok)return out;
    }
    if(input.at(0)=='0')//pre prefix 0x, 0b, etc
    {
        input.remove(0,1);//get rid it
    }
    if(input.at(0) == 'b'){

        input.remove(0,1);
        out = input.toInt(ok,2);
        if(*ok)return out;
    } else if(input.at(0) == 'o')
    {
        input.remove(0,1);
        out = input.toInt(ok,8);
        if(*ok)return out;

    } else
    {
        if(input.at(0) == 'x')
        {
            input.remove(0,1);
        }
        out = input.toInt(ok,16);//this is the default
        if(*ok)return out;
    }
    return 0;
}




}
