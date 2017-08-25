#ifndef STATUS_H
#define STATUS_H
#include <QStatusBar>

class Status : public QStatusBar
{
public:
    explicit Status(QWidget *parent = 0);
public slots:
    void setMessage(int x);

};

#endif // STATUS_H
