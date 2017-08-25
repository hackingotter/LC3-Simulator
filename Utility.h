#ifndef UTILITY_H
#define UTILITY_H
#include <QString>
#include <QObject>
#include <QSysInfo>
extern "C" {
#include "Registers.h"
#include "Memory.h"
#include "Simulator.h"
#include "Util.h"
}

#define RUBBERDUCKY true
#define BUBBLES true
#define BATHTIME(STRING) if(RUBBERDUCKY){qInfo(qPrintable(STRING));}
#ifdef RUBBERDUCKY
#define BUBBLEBATHTIME(STRING) if(RUBBERDUCKY && BUBBLES){ qInfo(qPtrintable(STRING));}
#endif
#ifndef MEMSIZE
#define MEMSIZE 65535
#endif
#define BANNERSIZE 50
#define BATHTIMEBANNER(MESSAGE)\
    BATHTIME(QString("-").repeated(BANNERSIZE))\
    BATHTIME(QString("|"+QString(" ").repeated(BANNERSIZE-2)+"|")\
    .replace(BANNERSIZE/2-(QString(MESSAGE).length()/2),QString(MESSAGE).length(),MESSAGE))\
    BATHTIME(QString("-").repeated(BANNERSIZE))
#ifndef DEFAULT_TEXT_HEIGHT
#define DEFAULT_TEXT_HEIGHT 20
#endif /* DEFAULT_TEXT_HEIGHT*/

#define QSTRNUM(num) QString().setNum(num)

#define CONNECT(sender,signal,receiver,slot) QObject::connect(sender,SIGNAL(signal),receiver,SLOT(slot));

namespace Utility {

inline QString name_or_addr(mem_addr_t target)
{
    label_t* label = getMemLabel(target);

    if(label!=nullptr)
    {
       return QString(label->name);
    }
    else
    {
       return  getHexString(target);
    }
    return "";
}
inline char * QString2CharPtr(QString source)
{
    char * target;
    std::string temp= source.toStdString();
    target = new char[temp.size()+1];
    strcpy(target, temp.c_str() );
    return target;
}

inline int QSTRING2INTBASE(QString source,int suggestedBase)
{
    BATHTIME("Looks like we are changing a QString to a Int ")
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

inline void systemInfoDebug()
{
    BATHTIME(QString("The architecture this was built on : ").append(QSysInfo::buildCpuArchitecture()))
    BATHTIME(QString("The architecture this is running on: ").append(QSysInfo::currentCpuArchitecture()))
    BATHTIME(QString("The computer: ").append(QSysInfo::prettyProductName()))
}

#define QVARIANT2VAL_T(QVARIANT) (static_cast<val_t>(QVARIANT.toString().toInt(nullptr,16)))
#define CHARPTR2QSTRING(CHARPTR)(QString::fromLocal8Bit(CHARPTR))

#define BADOP "Bad Op"
inline QString addr2Mnem(mem_addr_t addr)
{

    val_t val = getMemValue(addr);
    addr +=1;
    QString out;
    int imm5        =(val&0x001F);
    int reg11       =((val&0x0E00) >> 9);
    int reg8        =((val&0x01E0) >> 6);
    int reg2        =((val&0x0007) >> 0);
    bool zero543    =(val&0x0038);
    bool imm5YN     =(val&0x0020) >> 5;

    //It is much easier to be able to handle them without needing to account for names
    switch(val&0xF000)
    {
    case andOpCode  :out.append("AND ");break;
    case addOpCode  :out.append("ADD ");break;
    case brOpCode   :out.append("BR");break;
    case jmpOpCode  :out.append("JMP ");break;
    case jsrOpCode  :out.append("JSR");break;
    case ldOpCode   :out.append("LD ");break;
    case ldiOpCode  :out.append("LDI ");break;
    case ldrOpCode  :out.append("LDR ");break;
    case leaOpCode  :out.append("LEA ");break;
    case notOpCode  :out.append("NOT ");break;
    case rtiOpCode  :out.append("RTI ");break;
    case stOpCode   :out.append("ST ");break;
    case stiOpCode  :out.append("STI ");break;
    case strOpCode  :out.append("STR ");break;
    case trapOpCode :out.append("TRAP ");break;
    }

    switch(val& 0xF000)
    {
    case addOpCode:
        if((val&0x0038) ==0x0010)
        {
            out = QString("SUB ");

        }

    case andOpCode:
    {
        if((((val&0x0020)&&(val&0x0008))||(val&0x0020))) out = BADOP;
        else
        {
        out.append("R" + QSTRNUM(reg11) + ", R"+QSTRNUM(reg8));
        if(imm5YN)
        {
            out.append(", #").append(QSTRNUM(imm5));
        }
        else
        {
            out.append(", R").append(QSTRNUM(reg2));
        }
        }
        break;
    }
    case brOpCode :
    {
        if((val&0x0E00)==0x0E00)//if all three are set, just display BR
        {}
        else
            if(val&0x0E00)
        {
            if(val&0x0800) out.append("n");
            if(val&0x0400) out.append("z");
            if(val&0x0200) out.append("p");
        }
        if((val&0x0E00)==0x0000)
        {
            out = BADOP;
        }
        else
        {
        mem_addr_t target = addr + val&0x00FF;
        label_t* label = getMemLabel(target);
        out.append(" ");
        if(label!=nullptr)
        {
        out.append(QString(label->name));
        }
        else
        {
        out.append(getHexString(target));
        }
        }
        break;

}
    case jmpOpCode:
    {
        if((val&0x0E3F))//if there are ones outside of the OpCode and BaseR
            //Bad Op
        {
        out = BADOP;
        }
        else if(val&0x01C0)//if so, RET is the proper memn
        {
        out = "RET";
        }
        else
        {
        out = QString("JMP R%1").arg(reg8);
        }

    break;
    }
    case jsrOpCode:
    {
        BATHTIME("JSR")
        if(val&0x0800)//11th slot 1 means jsr
        {

            mem_addr_t target = addr + val&0x07FF;
            out.append(" "+name_or_addr(target));
        }
        else if(!(val&0x0E3F))//or, it could be jsrr
        {
            out.append("R R"+QSTRNUM(reg8));
        }
        else
        {
            out = BADOP;
        }
        break;
    }
    case ldOpCode :
    case ldiOpCode:
    case leaOpCode:
    case stOpCode :
    case stiOpCode:
    {
        out.append("R" + QSTRNUM(reg11) + ", "+ name_or_addr(addr+val&0x00FF));
        break;
    }
    case ldrOpCode:
    case strOpCode:
    {
        out.append("R"+ QSTRNUM(reg11)+", R"+ QSTRNUM(reg8)+ ", "+QSTRNUM(val&0x003F));
        break;
    }
    case rtiOpCode:
    {if(val&0x0FFF) out = BADOP;break;}
    case trapOpCode:
    {
        if(val&0x0F00)
        {
            out = BADOP;
        }
        else
        {
            name_or_addr(val&0x00FF);//this code could be wrong
        }
                break;
        }
    }
    return out;
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
};
#endif // UTILITY_H
