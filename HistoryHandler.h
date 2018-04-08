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
    void redo(int level = 0);
    void undo(int level = 0);
    void update(){qDebug("Stack has been changed; update");};
    bool add(QUndoCommand *cmd);
public slots:
    void setIndex(int idx) ;
};



#endif // HISTORYHANDLER_H
