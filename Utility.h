#ifndef UTILITY_H
#define UTILITY_H
#include <QString>
#include <QObject>
#include <QSysInfo>
//#include "Simulator.h"
#include "Util.h"

#include <QDebug>

#define RUBBERDUCKY true
#define BUBBLES true





#ifndef MEMSIZE
#define MEMSIZE 65535
#endif
#ifndef DEFAULT_TEXT_HEIGHT
#define DEFAULT_TEXT_HEIGHT 20
#endif /* DEFAULT_TEXT_HEIGHT*/

#define QSTRNUM(num) QString().setNum(num)

#define CONNECT(sender,signal,receiver,slot) QObject::connect(sender,SIGNAL(signal),receiver,SLOT(slot));

#define CHARPTR2QSTRING(CHARPTR)(QString::fromLocal8Bit(CHARPTR))

namespace Utility {

static bool* doing;

int QSTRING2INTBASE(QString source,int suggestedBase);

void systemInfoDebug();


}
#define BADOP "Bad Op"
typedef enum doPriority{
    subsysLevel=-1,//for during execution of single line
    systLevel = 0,//for when the system does something
    userLevel = 1,

};
#endif // UTILITY_H
