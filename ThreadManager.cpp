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
    qDebug("Done");
    emit stopped();//now we are done, time to tell everyone else
}
