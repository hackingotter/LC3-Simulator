#ifndef BRIDGE_H
#define BRIDGE_H
#include "ThreadManager.h"

#include <QObject>
#include "Utility.h"
#include <QThread>

class ThreadManager;

class Bridge :public QObject
{
    Q_OBJECT
    ThreadManager::RunUntil runningMode;
public:


    Bridge(ThreadManager::RunUntil run);
    ~Bridge();

    static void doWork();
public slots:
    void process();
    void beginHalt();
signals:
    void finished();
    void error(QString err);

private:
    static bool isRunning;
    static bool isHalting;

};



#endif // BRIDGE_H
