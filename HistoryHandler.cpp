#include "HistoryHandler.h"
#include "Utility.h"
HistoryHandler::HistoryHandler()
{
doing = false;
}
void HistoryHandler::undo(int level)
{
    if(HistoryHandler::doing)return;
    doing = true;
    QUndoStack::undo();
    doing = false;
}
void HistoryHandler::redo(int level)
{
    if(doing)return;
    doing = true;
    QUndoStack::redo();
    doing = false;
}
void HistoryHandler::push(QUndoCommand *cmd)
{
    if(doing)return;
    doing = true;
    QUndoStack::push(cmd);
    doing = false;
}
