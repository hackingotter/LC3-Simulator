#ifndef THREADMANAGER_H
#define THREADMANAGER_H

#include <QObject>
#include <QThread>
#include "computer.h"
class Bridge;
class ThreadManager : public QObject
{
    Q_OBJECT
public:
    enum RunUntil
    {
        Step    = 0,
        Next    = 1,
        Flag    = 2,
        Done    = 3
    };
    explicit ThreadManager(QObject *parent = 0);

signals:
    void started();
    void stopped();
    void requestHalt();
public slots:
    void activate(ThreadManager::RunUntil runningMode = Step);
private slots:
    void done();

};

#endif // THREADMANAGER_H
