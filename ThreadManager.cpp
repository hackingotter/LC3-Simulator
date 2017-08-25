#include "ThreadManager.h"
#include "Utility.h"
#include "Memory.h"
#include "Registers.h"

ThreadManager::ThreadManager(QObject *parent) : QObject(parent)
{
    BATHTIMEBANNER("Initializing Thread")
}

void ThreadManager::activate(int runningMode)
{
    BATHTIME("Activating Thread with number " + QString().setNum(runningMode));
    emit started(); // for safety reasons, we will say we started before we actually start
    old_Mem_State = getAllMemValues();
    old_Reg_State = getAllRegisters();
    Bridge* worker = new Bridge(runningMode);
    QThread *thread = new QThread();
    worker->moveToThread(thread);


    QObject::connect(thread, SIGNAL(started()) , worker, SLOT(process()));
    QObject::connect(worker, SIGNAL(finished()), thread, SLOT(quit()));
    QObject::connect(worker, SIGNAL(finished()), worker, SLOT(deleteLater()));
    QObject::connect(worker, SIGNAL(finished()), this,   SLOT(done()));

    //The next line makes it so that the thread deletes itself without crashes
    QObject::connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

    thread->start();
}

void ThreadManager::done()
{
    BATHTIME("Checking for changes")
    val_t* new_Mem_State = getAllMemValues();
    val_t* new_Reg_State = getAllRegisters();
    for(int i = 0;i<65535;i++)
    {
        if(old_Mem_State[i]!=new_Mem_State[i])
        BATHTIME(QString(getHexString(i)))
    }



    emit stopped();//now we are done, time to tell everyone else
}
