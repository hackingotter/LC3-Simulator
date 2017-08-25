#ifndef THOUGHT_H
#define THOUGHT_H

#include <QObject>

class Thought : public QObject
{
    Q_OBJECT

    Thought* next;
    Thought* past;
public:

    explicit Thought(QObject *parent = 0);

signals:

public slots:
};

#endif // THOUGHT_H
