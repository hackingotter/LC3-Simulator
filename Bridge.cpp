#include "Bridge.h"
#include "Utility.h"
#include <QObject>
#include <QThread>

#include "ThreadManager.h"
//#include "Simulator.h"

bool Bridge::isRunning = false;






Bridge::Bridge(int protocol) : QObject(nullptr)
{
    old_Mem_State = Computer::getDefault()->getAllMemValues();
    old_Reg_State = Computer::getDefault()->getAllRegisters();
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
        BATHTIME("Already running")
                return;
    }
    Bridge::isRunning = true;
    BATHTIME(QString().setNum(runningMode))
    BATHTIME(QString().setNum(Computer::getDefault()->getProgramStatus()))
    int *ok = new int(0);
    switch(runningMode)
    {
    case Next:
    {


        val_t target_PC = Computer::getDefault()->getRegister(PC)+1;

        BATHTIME("Am I done? Starting at"+ QString().setNum(target_PC))
        while((Computer::getDefault()->getRegister(PC)!= target_PC)&&(*ok!=1))
        {
            BATHTIME("Trying at "+ QString().setNum(Computer::getDefault()->getRegister(PC)))
            Computer::getDefault()->executeSingleInstruction();
            BATHTIME("Executed with error of " +QString().setNum(*ok))
        }
        BATHTIME("Done!")
        break;
    }
    case Step:
    {
        BATHTIME("Steppin")

        Computer::getDefault()->executeSingleInstruction();
        BATHTIME(QString().setNum(*ok))
        break;
    }
    default:BATHTIME("Welp")

    }

    BATHTIME("Hiya")

    BATHTIME("AGAIN")
    emit finished();
    isRunning=false;
}


