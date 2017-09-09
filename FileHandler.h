#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <QObject>
#include <QFile>
class FileHandler : public QObject
{
    Q_OBJECT
     // Leaving this behind comment since I would not start off with this.
     // Still, it might be interesting for your use case.
     // Q_PROPERTY(QByteArray data READ data WRITE setData NOTIFY dataChanged)

    Q_INVOKABLE QByteArray read();
    QByteArray data() const ;

public:
    explicit FileHandler(QObject *parent = 0);
    void write(QByteArray data);
signals:

public slots:
};

#endif // FILEHANDLER_H
