#ifndef BRIDGE_H
#define BRIDGE_H

#include <QObject>
#include "Utility.h"
#include <QThread>

class Bridge :public QObject
{
    Q_OBJECT
int runningMode;
public:
    enum Mode
    {
        Step = 0,
        Next = 1,
        Done = 2,
        Break = 3
    };

    Bridge(int run);
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
