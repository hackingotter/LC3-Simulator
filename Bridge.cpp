#include "Bridge.h"
#include "Utility.h"
#include <QObject>
#include <QThread>
#include "Console.h"
#include "ThreadManager.h"
//#include "Simulator.h"
#include "computer.h"
bool Bridge::isRunning = false;

bool Bridge::isHalting = false;




Bridge::Bridge(ThreadManager::RunUntil protocol) : QObject(nullptr)
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

//    int ok =0;
    switch(runningMode)
    {
    case ThreadManager::RunUntil::Next:
    {
        val_t target_PC = Computer::getDefault()->getRegister(PC)+1;

        // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        // I added a function called executeUntilAddress() that does this.
        // Logic like this belongs into the computer
        // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

        // TODO update

        qDebug("Am I done? Starting at"+ QString().setNum(target_PC).toLocal8Bit());
        Computer::getDefault()->executeUntilAddress(target_PC);

        Bridge::isHalting = false;

        break;
    }
    case ThreadManager::RunUntil::Step:
    {
        qDebug("Steppin");

        executeCommand(step,nullptr);
//        qDebug(QString().setNum(&ok).toLocal8Bit());
        break;
    }
    case ThreadManager::RunUntil::Flag:
    {
        qDebug("Until Break");
        executeCommand(_continue,nullptr);
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


