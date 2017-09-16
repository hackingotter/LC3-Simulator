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
        Done = 2
    };

    Bridge(int run);
    ~Bridge();

    static void doWork();
public slots:
    void process();
signals:
    void finished();
    void error(QString err);

private:
    static bool isRunning;

};



#endif // BRIDGE_H
