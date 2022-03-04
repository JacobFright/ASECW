#ifndef RETURNMESSAGE_H
#define RETURNMESSAGE_H

#include <QThread>
#include <QObject>
#include <QTcpSocket>
#include <QThreadPool>
#include <QRunnable>
#include <QDebug>

class returnMessage : public QThread, public QRunnable
{
    Q_OBJECT
public:
    explicit returnMessage(QTcpSocket *socket, QStringList message, QObject *parent = 0);
    void run();

private:
    QTcpSocket *socket;
    QStringList message;
};

#endif // RETURNMESSAGE_H
