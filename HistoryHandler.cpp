//
//  HistoryHandler.cpp
//  LC3-Sim
//
//  Created by Joseph Melberg on 1/1/18.
//  Copyright © 2018 EIU. All rights reserved.
//

#include "HistoryHandler.h"
#include "Utility.h"
#include "QUndoStack"

HistoryHandler::HistoryHandler()
{
    HistoryHandler::doing = 0;
}

void HistoryHandler::setIndex(int idx)
{
    qDebug("Helloooooo");
    HistoryHandler::doing++;
    QUndoStack::setIndex(idx);
    doing--;
}
 int HistoryHandler::doing = 0;
void HistoryHandler::undo(int level)
{
//    if(dont)return;
    doing++;

    QUndoStack::undo();

            qDebug(QString().setNum(count()).toLocal8Bit());
            doing--;
}

const QUndoCommand *HistoryHandler::command(int index) const
{
    return  QUndoStack::command(index);
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



