#ifndef HISTORYHANDLER_H
#define HISTORYHANDLER_H
#include <QUndoCommand>
#include <QUndoStack>
#include <QStack>
#include "Utility.h"


#define IFCANUPDATE(DO)\
    if(!HistoryHandler::doing)\
    {\
    DO\
    }


class HistoryHandler: public QUndoStack
{
    Q_OBJECT


public:
    static int doing;
    HistoryHandler();

    void update(){qDebug("Stack has been changed; update");};
    void push(QUndoCommand *cmd);
signals:
    void flare();
    void masked(int);
    void maskedQString(QString&);
    void unmasked(int);
    void unmaskedQString(QString*);
public slots:
    void redo();

    void undo();
    void setIndex(int idx) ;
    void unmask();
    void mask();
};



#endif // HISTORYHANDLER_H
