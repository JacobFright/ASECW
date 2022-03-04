#include "listenthread.h"

/**
 * @brief ListenThread::ListenThread
 * @param socket
 * @param ID
 * @param parent
 */
ListenThread::ListenThread(QTcpSocket *socket, int ID, QObject *parent) :
    QThread(parent)
{
    this->socketDescriptor = ID;
    this->socket = socket;
}

/**
 * @brief ListenThread::run
 */
void ListenThread::run(){
    //thread starts here
    //    socket = new QTcpSocket();
    //    if(!socket->setSocketDescriptor(this->socketDescriptor)){
    //        emit error(socket->error());
    //        return;
    //    }

    connect(socket,SIGNAL(readyRead()),this,SLOT(readyRead()),Qt::DirectConnection);
    connect(socket,SIGNAL(disconnected()),this,SLOT(disconnected()),Qt::DirectConnection);

    qDebug() << "Client Connected on socket: " << socketDescriptor;
    emit update("Client Connected");

    exec();
}

/**
 * @brief ListenThread::ReturnResult
 * @param Result
 */
void ListenThread::ReturnResult(QStringList Result){

    QByteArray Buffer;
    for(int i = 0; i < Result.length(); i++){
        Buffer.append(Result[i]);
    }

//    if(Result[0] == "INCOMINGMESSAGE:"){
//        QTcpSocket *sendto = new QTcpSocket();
//        if(!sendto->setSocketDescriptor(Result[1].toInt())){
//            emit error(sendto->error());
//            return;
//        }
//        bool connected = (sendto->state() == QTcpSocket::ConnectedState);
//        qDebug() << "MESSAGE REUTRNED TO SOCKET: " + Result[1] + " SOCKET CONNECTIONSTATE: " + connected;
//        sendto->flush();
//        sendto->write(Buffer);
//        sendto->waitForBytesWritten();
//        return;
//    }
//    else{
        qDebug() << socketDescriptor << " Data out: " << Buffer;
        socket->write(Buffer);
//    }
}

/**
 * @brief ListenThread::readyRead
 */
void ListenThread::readyRead(){
    QByteArray Data = socket->readAll();
    qDebug() << socketDescriptor << " Data in: " << Data;
    handleMessageThread *handleMessage = new handleMessageThread(socketDescriptor, Data,this);
    handleMessage->setAutoDelete(true);
    connect(handleMessage, SIGNAL(ReturnResult(QStringList)),this, SLOT(ReturnResult(QStringList)),Qt::QueuedConnection);
    connect(handleMessage, SIGNAL(ReturnMessage(QStringList)),this, SLOT(ReturnMessage(QStringList)),Qt::QueuedConnection);
    QThreadPool::globalInstance()->start(handleMessage);
}

/**
 * @brief ListenThread::ReturnMessage
 * @param Result
 */
void ListenThread::ReturnMessage(QStringList Result){
    emit sendMessage(Result);
}

/**
 * @brief ListenThread::disconnected
 */
void ListenThread::disconnected(){
    qDebug() << socketDescriptor << " Disconnected";
    socket->deleteLater();
    exit(0);
}

