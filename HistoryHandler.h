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
    void redo(int level = 0);
    void undo(int level = 0);
    void update(){qDebug("Stack has been changed; update");};
    void push(QUndoCommand *cmd);
};

#endif // HISTORYHANDLER_H
