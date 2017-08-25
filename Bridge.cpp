#include "Bridge.h"
#include "Utility.h"
#include <QObject>
#include <QThread>

#include "ThreadManager.h"
#include "Simulator.h"
#include "Registers.h"
#include "Memory.h"

bool Bridge::isRunning = false;






Bridge::Bridge(int protocol) : QObject(nullptr)
{
    old_Mem_State = getAllMemValues();
    old_Reg_State = getAllRegisters();
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
    BATHTIME(QString().setNum(getProgramStatus()))
    int *ok = new int(0);
    switch(runningMode)
    {
    case Next:
    {


        val_t target_PC = getRegister(PC)+1;

        BATHTIME("Am I done? Starting at"+ QString().setNum(target_PC))
        while((getRegister(PC)!= target_PC)&&(*ok!=1))
        {
            BATHTIME("Trying at "+ QString().setNum(getRegister(PC)))
            executeSingleInstruction();
            BATHTIME("Executed with error of " +QString().setNum(*ok))
        }
        BATHTIME("Done!")
        break;
    }
    case Step:
    {
        BATHTIME("Steppin")

        executeSingleInstruction();
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


