#include "Bridge.h"
#include "Utility.h"
#include <QObject>
#include <QThread>

#include "ThreadManager.h"
//#include "Simulator.h"
#include "computer.h"
bool Bridge::isRunning = false;

bool Bridge::isHalting = false;




Bridge::Bridge(int protocol) : QObject(nullptr)
{
    runningMode = protocol;
}
Bridge::~Bridge()
{
    Bridge::isRunning =false;
}
void Bridge::doWork()
{
//    emit simPrep();
//    QThread::sleep(4);
}

void Bridge::process()
{
    if(Bridge::isRunning)
    {
        qDebug("Already running");
                return;
    }
    Bridge::isRunning = true;

    int *ok = new int(0);
    switch(runningMode)
    {
    case Next:
    {
        val_t target_PC = Computer::getDefault()->getRegister(PC)+1;

        // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        // I added a function called executeUntilAddress() that does this.
        // Logic like this belongs into the computer
        // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

        // TODO update

        qDebug("Am I done? Starting at"+ QString().setNum(target_PC).toLocal8Bit());
        while((Computer::getDefault()->getRegister(PC)!= target_PC)&&(*ok !=1)&&!Bridge::isHalting)
        {
            qDebug("Trying at "+ QString().setNum(Computer::getDefault()->getRegister(PC)).toLocal8Bit());
            Computer::getDefault()->executeSingleInstruction();
            qDebug("Executed with error of " +QString().setNum(*ok).toLocal8Bit());
        }
        Bridge::isHalting = false;
        qDebug("Done!");
        break;
    }
    case Step:
    {
        qDebug("Steppin");

        Computer::getDefault()->executeSingleInstruction();
        qDebug(QString().setNum(*ok).toLocal8Bit());
        break;
    }
    default:qDebug("Welp");

    }

    qDebug("Hiya");

    qDebug("AGAIN");
    emit finished();
    isRunning=false;
}
void Bridge::beginHalt()
{
    Bridge::isHalting = true;
}


