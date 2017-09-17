#include "HistoryHandler.h"
#include "Utility.h"

HistoryHandler::HistoryHandler()
{
doing = false;
}
void HistoryHandler::undo(int level)
{
//    if(dont)return;
//    QUndoStack::undo();
}
void HistoryHandler::redo(int level)
{
//    if(Utility::remember)return;
//    QUndoStack::redo();
}
void HistoryHandler::push(QUndoCommand *cmd)
{
//    if(Utility::remember)return;
//    QUndoStack::push(cmd);
}
