#include "returnmessage.h"

/**
 * @brief returnMessage::returnMessage
 * @param lsocket
 * @param message
 * @param parent
 */
returnMessage::returnMessage(QTcpSocket *lsocket, QStringList message, QObject *parent)
{
    this->socket = lsocket;
    this->message = message;
}

/**
 * @brief returnMessage::run
 */
void returnMessage::run(){
    QByteArray Buffer;
    for(int i = 0; i < message.length(); i++){
        Buffer.append(message[i]);
    }
    socket->write(Buffer);
    socket->flush();
}
