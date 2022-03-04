#include "myserver.h"

/**
 * @brief MyServer::MyServer
 * @param parent
 */
MyServer::MyServer(QObject *parent)
{
    QThreadPool::globalInstance()->setMaxThreadCount(20);
}

/**
 * @brief MyServer::StartServer
 */
void MyServer::StartServer()
{
    createDatabase();
    if(!this->listen(QHostAddress::Any,8080)){
        qDebug() << "Could not start server";
    }
    else{
        qDebug() << "Server is listening... ";
    }
}

/**
 * @brief MyServer::incomingConnection
 * @param socketDescriptor
 */
void MyServer::incomingConnection(qintptr socketDescriptor){
    qDebug() << socketDescriptor << " Connecting...";
    this->socketDescriptor = socketDescriptor;
    QTcpSocket *socket = new QTcpSocket();
    if(!socket->setSocketDescriptor(this->socketDescriptor)){
        qDebug() << socket->error();
        return;
    }
    ListenThread *thread = new ListenThread(socket, socketDescriptor, this);
    connectedClients.append(socket);
    connect(thread, SIGNAL(finished()),thread, SLOT(deleteLater()));
    connect(thread, SIGNAL(sendMessage(QStringList)),this,SLOT(returnMessage(QStringList)));
    QThreadPool::globalInstance()->start(thread);
}

/**
 * @brief MyServer::returnMessage
 * @param message
 */
void MyServer::returnMessage(QStringList message){
    qDebug() << message;
    for(int i = 0; i < connectedClients.length(); i++){
        if(connectedClients[i]->socketDescriptor() == message[1].toInt()){
            class returnMessage *messagethread = new class returnMessage(connectedClients[i], message, this);
            QThreadPool::globalInstance()->start(messagethread);
        }
        else if(message[0] == "USERONLINE"){
            class returnMessage *messagethread = new class returnMessage(connectedClients[i], message, this);
            QThreadPool::globalInstance()->start(messagethread);
        }
    }
}

/**
 * @brief MyServer::createDatabase
 */
void MyServer::createDatabase(){
    const char *dataConnectName = "information_schema";
    const char *dataBaseName = "Chats";
    const char *userName = "admin";
    const char *userPassword = "fSjZA8dFvjMi";
    const char *server = "127.0.0.1";
    MYSQL *conn, mysql;
    int query_state;

    conn = mysql_init(&mysql);
    conn = mysql_real_connect(&mysql, server, userName, userPassword,
                              dataConnectName, 0, 0, 0);
    if (conn == NULL) {
        qDebug() << mysql_error(&mysql);
        return;
    } else {
        qDebug() << "Database connection successful.";
    }
    std::string create = "CREATE DATABASE IF NOT EXISTS Chats";

    query_state = mysql_query(conn, create.c_str());
    if (query_state != 0) {
        qDebug() << mysql_error(&mysql);
        return;
    } else {
        qDebug() <<"Chats database successfully created.";
    }
    mysql_close(conn);

    conn = mysql_init(&mysql);
    conn = mysql_real_connect(&mysql, server, userName, userPassword,
                              dataBaseName, 0, 0, 0);
    if (conn == NULL) {
        qDebug() << mysql_error(&mysql);
        return;
    } else {
        qDebug() << "Connected to Chats database successful.";
    }

    std::string tables[5];
    std::string tableNames[5] = {"Users","Chats","Messages","Participants","Contacts"};

    tables[0] =
            "CREATE TABLE IF NOT EXISTS Users (UserID int NOT NULL AUTO_INCREMENT, Name varchar(50), Username varchar(50), "
            "Pword varchar(50), IsActive TINYINT, LastActive datetime, SocketID INT, PRIMARY KEY (UserID));";
    tables[1] = "CREATE TABLE IF NOT EXISTS Chats (ChatID int NOT NULL AUTO_INCREMENT, Title varchar(255),"
                " PRIMARY KEY (ChatID));";
    tables[2] =
            "CREATE TABLE IF NOT EXISTS Messages (MessageID int NOT NULL AUTO_INCREMENT, SenderID int, ChatID int, SendTime datetime,"
            "Message varchar(255),"
            " PRIMARY KEY (MessageID), FOREIGN KEY (SenderID) REFERENCES Users(UserID),"
            "FOREIGN KEY (ChatID) REFERENCES Chats(ChatID));";
    tables[3] =
            "CREATE TABLE IF NOT EXISTS Participants (ParticipantID int NOT NULL AUTO_INCREMENT, ChatID int, UserID int,"
            " PRIMARY KEY (ParticipantID), FOREIGN KEY (ChatID) REFERENCES Chats(ChatID),"
            "FOREIGN KEY (UserID) REFERENCES Users(UserID));";
    tables[4] = "CREATE TABLE IF NOT EXISTS Contacts (ContactID int NOT NULL AUTO_INCREMENT, ContactUserID int, UserID int,"
                " PRIMARY KEY (ContactID),"
                " FOREIGN KEY (ContactUserID) REFERENCES Users(UserID),"
                "FOREIGN KEY (UserID) REFERENCES Users(UserID));";

    for (int i = 0; i < 5; i++) {
        query_state = mysql_query(conn, tables[i].c_str());
        if (query_state != 0) {
            qDebug() << mysql_error(&mysql);
            return;
        } else {
            std::string success = "Table " + tableNames[i] + " created successfully.";
            qDebug() << QString::fromStdString(success);
        }
    }

    qDebug() << "All tables created successfully";
    mysql_close(conn);
}
