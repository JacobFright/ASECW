#ifndef MYSERVER_H
#define MYSERVER_H

#include <QTcpServer>
#include <QDebug>
#include <QThreadPool>
#include "listenthread.h"
#include "returnmessage.h"

class MyServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit MyServer(QObject *parent = 0);
    void createDatabase();
    void StartServer();
signals:
public slots:
    void returnMessage(QStringList);
protected:
    void incomingConnection(qintptr socketDescriptor);
private:
    int socketDescriptor;
    QVector<QTcpSocket*> connectedClients;
};

#endif // MYSERVER_H
