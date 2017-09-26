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



//cond_t RegisterModel::handle_CC_RegisterView_Input(QString in)
//{
//    BATHTIME("Handling PSR")
//    QString lowered = in.toLower();
//    BATHTIME("Checking if it is a register")
//    if(lowered.startsWith("r"))
//    {
//        BATHTIME("It might be")
//        if(lowered.length()==2)
//        {
//            BATHTIME("It looks like it")
//            QString ch = lowered.at(1);
//            int reg_num;
//            ch.number(reg_num,16);
//            if(reg_num<9)
//            {
//                BATHTIME("It really is!")
//                val_t val = getRegister(static_cast<reg_t>(reg_num));
//                if(val<0)return cond_n;
//                if(val>0)return cond_p;
//                if(val==0)return cond_z;
//            }
//            BATHTIME("Nope")
//        }
//        BATHTIME("Nadda")
//    }
//    //Handle negative
//    PSR_HANDLER(lowered,"n"     ,   cond_n)
//    PSR_HANDLER(lowered,"negative" ,cond_n)
//    PSR_HANDLER(lowered,":("    ,   cond_n)
//    PSR_HANDLER(lowered,":-("   ,   cond_n)
//    PSR_HANDLER(lowered,"neg"   ,   cond_n)
//    PSR_HANDLER(lowered,"en"    ,   cond_n)
//    PSR_HANDLER(lowered,"nn"    ,   cond_n)
//    PSR_HANDLER(lowered,"nnn"   ,   cond_n)
//    PSR_HANDLER(lowered,"-"     ,   cond_n)
//    //Handle positive
//    PSR_HANDLER(lowered,"p"     ,   cond_p)
//    PSR_HANDLER(lowered,"positive" ,cond_p)
//    PSR_HANDLER(lowered,":)"    ,   cond_p)
//    PSR_HANDLER(lowered,":-)"   ,   cond_p)
//    PSR_HANDLER(lowered,"pos"   ,   cond_p)
//    PSR_HANDLER(lowered,"po"    ,   cond_p)
//    PSR_HANDLER(lowered,"pp"    ,   cond_p)
//    PSR_HANDLER(lowered,"ppp"   ,   cond_p)
//    PSR_HANDLER(lowered,"+"     ,   cond_p)
//    //Handle zero
//    PSR_HANDLER(lowered,"z"     ,   cond_z)
//    PSR_HANDLER(lowered,"zero"  ,   cond_z)
//    PSR_HANDLER(lowered,":|"    ,   cond_z)
//    PSR_HANDLER(lowered,":-|"   ,   cond_z)
//    PSR_HANDLER(lowered,"zer"   ,   cond_z)
//    PSR_HANDLER(lowered,"ze"    ,   cond_z)
//    PSR_HANDLER(lowered,"zz"    ,   cond_z)
//    PSR_HANDLER(lowered,"zzz"   ,   cond_z)
//    PSR_HANDLER(lowered,""      ,   cond_z)


//    if(lowered.toInt()>0)return     cond_p;
//    if(lowered.toInt()<0)return     cond_n;


//    return cond_z;
//}
}
