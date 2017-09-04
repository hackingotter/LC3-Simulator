#ifndef UTILITY_H
#define UTILITY_H
#include <QString>
#include <QObject>
#include <QSysInfo>
//#include "Simulator.h"
#include "Util.h"
#include <QColor>
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
int qColor2Int(QColor color);
QColor int2QColor(int color);
}
#define BADOP "Bad Op"

#endif // UTILITY_H
