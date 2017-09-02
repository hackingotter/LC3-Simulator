#include "ThreadManager.h"
#include "Utility.h"


ThreadManager::ThreadManager(QObject *parent) : QObject(parent)
{
    qDebug("Initializing Thread");
}

void ThreadManager::activate(int runningMode)
{
    qDebug("Activating Thread with number " + QString().setNum(runningMode).toLocal8Bit());
    emit started(); // for safety reasons, we will say we started before we actually start
    old_Mem_State = Computer::getDefault()->getAllMemValues();
    old_Reg_State = Computer::getDefault()->getAllRegisters();
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
    qDebug("Checking for changes");
    val_t* new_Mem_State = Computer::getDefault()->getAllMemValues();
    val_t* new_Reg_State = Computer::getDefault()->getAllRegisters();
    for(int i = 0;i<65535;i++)
    {
        if(old_Mem_State[i]!=new_Mem_State[i])
        qDebug(QString(getHexString(i)).toLocal8Bit());
    }



    emit stopped();//now we are done, time to tell everyone else
}
