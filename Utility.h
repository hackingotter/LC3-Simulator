#ifndef UTILITY_H
#define UTILITY_H
#include <QString>
#include <QObject>
#include <QSysInfo>
//#include "Simulator.h"
#include "Util.h"
#include "computer.h"

#define RUBBERDUCKY true
#define BUBBLES true

#ifdef RUBBERDUCKY
#define BATHTIME(STRING) qInfo(qPrintable(STRING));
#else
#define BATHTIME(STRING)
#endif

#ifdef RUBBERDUCKY
#define BUBBLEBATHTIME(STRING) if(RUBBERDUCKY && BUBBLES){ qInfo(qPtrintable(STRING));}
#else
#define BUBBLEBATHTIME(STRING)
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

#define CHARPTR2QSTRING(CHARPTR)(QString::fromLocal8Bit(CHARPTR))

namespace Utility {


int QSTRING2INTBASE(QString source,int suggestedBase);

void systemInfoDebug();
QString addr2Mnem(mem_addr_t addr);
QString name_or_addr(mem_addr_t target);
}

#endif // UTILITY_H
