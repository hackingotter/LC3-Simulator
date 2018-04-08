#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include <QObject>

class StateMachine : public QObject
{
    Q_OBJECT
public:
    explicit StateMachine(QObject *parent = nullptr);

signals:

public slots:
};

#endif // STATEMACHINE_H