#ifndef UTILITY_H
#define UTILITY_H
#include <QString>
#include <QObject>
#include <QSysInfo>
//#include "Simulator.h"
#include "Util.h"
#include <QColor>
#include <QDebug>
#include "QShortcut"

#define ORGANIZATION "Melberg & Ott"
#define APPNAME "PennSim++"


#define RUBBERDUCKY true
#define BUBBLES true

#define ASSEMBLY_SUFFIX ".asm"

#define OBJECT_SUFFIX ".obj"

#define SYMBOL_SUFFIX ".sym"

#define COLORSLICE 0x1F
//#define color(in)((in/31.0)*255)
#define COLOR_SPLITTER(QCOLOR) (QCOLOR).red(),(QCOLOR).green(),(QCOLOR).blue()
#define PCCOLOR (QColor(255,255,0))
#define R0COLOR (QColor(0,102,204))
#define R1COLOR (QColor(255,102,205))
#define R2COLOR (QColor(255,153,51))
#define R3COLOR (QColor(153,51,153))
#define R4COLOR (QColor(102,255,51))
#define R5COLOR (QColor(153,102,51))
#define R6COLOR (QColor(255,0,255))

#ifndef MEMSIZE
#define MEMSIZE 65535
#endif
#ifndef DEFAULT_TEXT_HEIGHT
#define DEFAULT_TEXT_HEIGHT 20
#endif /* DEFAULT_TEXT_HEIGHT*/

#define QSTRNUM(num) QString().setNum(num)

#define CONNECT(sender,signal,receiver,slot) QObject::connect(sender,SIGNAL(signal),receiver,SLOT(slot));

#define CHARPTR2QSTRING(CHARPTR)(QString::fromLocal8Bit(CHARPTR))

#define ADDSHORTCUT(NAME,KEY,WHERE,FUNCTION)\
{\
    QShortcut* cut = new QShortcut(WHERE);\
    cut->setKey(KEY);\
    connect(cut,&QShortcut::activated,WHERE, [=](){WHERE->FUNCTION;});\
    cut->setContext(Qt::WidgetShortcut);\
    cut->setObjectName(NAME);\
    }
namespace Utility {
//static bool remember = false;
class Utilit
{
public:
    static mem_addr_t specialSelectStart;
    static mem_addr_t specialSelectEnd;
    static void setup();
};

QString charToQString(char ch);
int QSTRING2INTBASE(QString source,int suggestedBase);

void systemInfoDebug();
QColor translater(val_t in);
int qColor2Int(QColor color);
QColor int2QColor(int color);

val_t unifiedInput2Val(QString input, bool* ok);
}
#define BADOP "Bad Op"

#endif // UTILITY_H
