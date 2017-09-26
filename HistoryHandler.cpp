#include "HistoryHandler.h"
#include "Utility.h"
#include "QUndoStack"

HistoryHandler::HistoryHandler()
{
doing = 0;
}
void HistoryHandler::undo(int level)
{
//    if(dont)return;
    doing++;
    QUndoStack::undo();
    qDebug(QString().setNum(count()).toLocal8Bit());
    doing--;
}
void HistoryHandler::redo(int level)
{
//    if(doing)return;
    doing++;
    QUndoStack::redo();
    doing--;
}
bool HistoryHandler::add(QUndoCommand *cmd)
{
//qDebug(cmd->actionText().toLocal8Bit()+ " " +QString().setNum(this->index()).toLocal8Bit());
if(doing==0)this->QUndoStack::push(cmd);
//qDebug("hy");
return true;
}

