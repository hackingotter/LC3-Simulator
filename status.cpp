#include "status.h"
#include <QStatusBar>
Status::Status(QWidget *parent):QStatusBar(parent)
{

}

void Status::setMessage(int x)
{
    QString str = QString("");
    str.setNum(x,10);
    QString str2 = QString("");
    str2.setNum(x,10);

    str.append(" ,");
    str.append(str2);
    str.append('\0');
    QStatusBar::showMessage(str.toLatin1());
}


