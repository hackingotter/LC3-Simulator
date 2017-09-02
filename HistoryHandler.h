#ifndef HISTORYHANDLER_H
#define HISTORYHANDLER_H
#include <QUndoCommand>
#include <QUndoStack>
#include <QStack>
#include "Utility.h"




class HistoryHandler: public QUndoStack
{
    Q_OBJECT
public:
     bool doing;

    HistoryHandler();
    void redo();
    void undo();
    void update(){qDebug("Stack has been changed; update");};
    void push(QUndoCommand *cmd);
};

#endif // HISTORYHANDLER_H
