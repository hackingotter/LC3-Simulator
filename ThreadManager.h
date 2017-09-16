#ifndef THREADMANAGER_H
#define THREADMANAGER_H

#include <QObject>
#include <QThread>
#include "Bridge.h"
#include "computer.h"
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
public slots:
    void activate(int runningMode = 0);
private slots:
    void done();

};

#endif // THREADMANAGER_H
