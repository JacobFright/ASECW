#ifndef HANDLEMESSAGETHREAD_H
#define HANDLEMESSAGETHREAD_H

#include <QThread>
#include <QTcpSocket>
#include <QDebug>
#include <QDateTime>
#include <QRunnable>
#include <mysql/mysql.h>


class handleMessageThread : public QThread , public QRunnable
{
    Q_OBJECT
public:
    explicit handleMessageThread(int socketDescriptor, QByteArray Buffer, QObject *parent = 0);

    handleMessageThread();
    void run();
    QString decryptpassword(QString password);
    QStringList regAccount(QString name, QString username, QString password);
    QByteArray Data;
    QStringList retrieveContacts(QString User);
    QStringList login(QString username, QString password);
    QStringList logout(QString username);
    QStringList searchUsers(QString User, QString criteria);
    QStringList addContact(QString contact, QString User);
    QStringList retrieveMessages(QString chatName);
    QStringList createGroup(QStringList participants);
    void userOnline(QStringList);
    void userOffline(QString);
    void sendMessage(QStringList MessageInfo);
    void handleMessage(QByteArray Data);

signals:
    void error(QTcpSocket::SocketError socketError);
    void ReturnResult(QStringList Result);
    void ReturnMessage(QStringList Message);
public slots:

private:
    const char *dataBaseName = "Chats";
    const char *userName = "admin";
    const char *userPassword = "fSjZA8dFvjMi";
    const char *server = "127.0.0.1";
    MYSQL *conn, mysql;
    MYSQL_RES *res;
    MYSQL_ROW row;
    int query_state;
    int socketDescriptor;
    QTcpSocket *socket;

};

#endif // HANDLEMESSAGETHREAD_H
