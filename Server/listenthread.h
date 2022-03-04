#ifndef LISTENTHREAD_H
#define LISTENTHREAD_H

#include <QThread>
#include <QObject>
#include <QTcpSocket>
#include <QThreadPool>
#include <QRunnable>
#include <QDebug>
#include "handlemessagethread.h"


class ListenThread : public QThread, public QRunnable
{
    Q_OBJECT
public:
    explicit ListenThread(QTcpSocket *socket, int ID, QObject *parent = 0);

    ListenThread();
    void run();

signals:
    void error(QTcpSocket::SocketError socketError);
    void Result(QByteArray);
    void update(QString update);
    void sendMessage(QStringList message);

public slots:
    void readyRead();
    void disconnected();
    void ReturnResult(QStringList Result);
    void ReturnMessage(QStringList Result);

private:
    QTcpSocket *socket;
    int socketDescriptor;

};

#endif // LISTENTHREAD_H
