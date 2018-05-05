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
    connect(this,SIGNAL(indexChanged(int)),this,SLOT(unmask()));
}
void HistoryHandler::mask()
{

    doing++;
    qDebug("Masking level:" + QString().setNum(doing).toLocal8Bit());

}
void HistoryHandler::unmask()
{

    qDebug("UnMasking level:"+QString().setNum(doing).toLocal8Bit());
doing--;
    if(doing<=0)
        doing = 0;


    if(doing==0)emit flare();


}
void HistoryHandler::setIndex(int idx)
{

    qDebug("Helloooooo");
    mask();
    QUndoStack::setIndex(idx);
    qDebug("Byeeeeeeeeee");
}
 int HistoryHandler::doing = 0;
void HistoryHandler::undo()
{
//    if(dont)return;
    mask();
qDebug("UNDOING");
    QUndoStack::undo();

            qDebug(QString().setNum(count()).toLocal8Bit());

}

void HistoryHandler::push(QUndoCommand *cmd)
{
    qDebug(QString().setNum(doing).toLocal8Bit());
    if(doing==0)
    {
        mask();
        qDebug("Pushing");
        this->QUndoStack::push(cmd);
        unmask();
    }
}
void HistoryHandler::redo()
{
//    if(doing)return;
    doing++;
    qDebug("DOING");
    QUndoStack::redo();
    doing--;
}
//bool HistoryHandler::add(QUndoCommand *cmd)
//{
////qDebug(cmd->actionText().toLocal8Bit()+ " " +QString().setNum(this->index()).toLocal8Bit());
//if(doing==0)this->QUndoStack::push(cmd);
////qDebug("hy");
//return true;
//}



