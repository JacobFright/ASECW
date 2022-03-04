#include "handlemessagethread.h"

/**
 * @brief handleMessageThread::handleMessageThread
 * @param socketDescriptor
 * @param Buffer
 * @param parent
 */
handleMessageThread::handleMessageThread(int socketDescriptor, QByteArray Buffer, QObject *parent):
    QThread(parent)
{
    this->Data = Buffer;
    this->socketDescriptor = socketDescriptor;
}

/**
 * @brief handleMessageThread::run
 */
void handleMessageThread::run(){
    handleMessage(Data);
}

/**
 * @brief handleMessageThread::login
 * @param username
 * @param password
 * @return
 */
QStringList handleMessageThread::login(QString username, QString password){
    QStringList login;
    login.append("LOGIN:");
    conn = mysql_init(&mysql);
    conn = mysql_real_connect(&mysql, server, userName, userPassword,
                              dataBaseName, 0, 0, 0);
    if (conn == NULL) {
        qDebug() << mysql_error(&mysql);
        login.append("0");
        return login;
    } else {
        qDebug() << "Database connection successful.";
    }

    QString query = "SELECT UserID, Pword FROM Users WHERE USERNAME = '" + username + "'";
    query_state = mysql_query(conn, query.toStdString().c_str());
    if (query_state != 0) {
        qDebug() << mysql_error(conn);
        login.append("0");
        return login;
    } else {
        res = mysql_store_result(conn);
        row = mysql_fetch_row(res);
        qDebug() << "Password retrieved: " << row[1];
    }
    mysql_close(conn);

    QString retrievedPword = row[1];
    //    if(password.endsWith('\n')){
    //    password.chop(1);
    //}

    if(retrievedPword == password){
        conn = mysql_init(&mysql);
        conn = mysql_real_connect(&mysql, server, userName, userPassword,
                                  dataBaseName, 0, 0, 0);
        QDateTime current = QDateTime::currentDateTime();
        query = "UPDATE Users SET ISACTIVE = 1, LASTACTIVE = UTC_TIMESTAMP(), SocketID = '" + QString::number(socketDescriptor) + "' WHERE USERID = '" + row[0] + "'";
        query_state = mysql_query(conn, query.toStdString().c_str());
        if (query_state != 0) {
            qDebug() << mysql_error(conn);
            login.append("0");
            return login;
        } else {
            qDebug() << "User Logged in";
        }
        mysql_close(conn);
        login.append("1:");
        login.append(row[0]);
        return login;
    }
    else{
        login.append("0");
        return login;
    }
}

/**
 * @brief handleMessageThread::logout
 * @param username
 * @return
 */
QStringList handleMessageThread::logout(QString username){
    QStringList logout;
    logout.append("LOGOUT:");
    conn = mysql_init(&mysql);
    conn = mysql_real_connect(&mysql, server, userName, userPassword,
                              dataBaseName, 0, 0, 0);
    if (conn == NULL) {
        qDebug() << mysql_error(&mysql);
        mysql_close(conn);
        logout.append("0");
        return logout;
    } else {
        qDebug() << "Database connection successful.";
    }

    QString query = "UPDATE Users SET IsActive = 0, SocketID = '-1' WHERE USERNAME = '" + username + "'";
    query_state = mysql_query(conn, query.toStdString().c_str());
    if (query_state != 0) {
        qDebug() << mysql_error(conn);
        mysql_close(conn);
        logout.append("0");
        return logout;
    } else {
        mysql_close(conn);
        logout.append("1");
        userOffline(username);
        return logout;
        qDebug() << "Password retrieved: " << row[3];
    }

}

/**
 * @brief handleMessageThread::regAccount
 * @param name
 * @param username
 * @param password
 * @return
 */
QStringList handleMessageThread::regAccount(QString name, QString username, QString password){
    QStringList registered;
    registered.append("REGISTER");
    conn = mysql_init(&mysql);
    conn = mysql_real_connect(&mysql, server, userName, userPassword,
                              dataBaseName, 0, 0, 0);
    if (conn == NULL) {
        qDebug() << mysql_error(&mysql);
        registered.append("0");
        return registered;
    } else {
        qDebug() << "Database connection successful.";
    }

    QString query = "SELECT Username FROM Users WHERE Username = '" + username + "'";

    query_state = mysql_query(conn, query.toStdString().c_str());


    if (query_state != 0) {
        qDebug() << mysql_error(conn);
        registered.append(":0");
        return registered;
    }
    else {
        res = mysql_store_result(conn);
        int rowcount = mysql_num_rows(res);
        qDebug() << row;
        if(rowcount > 0){
            qDebug() << "Row is greater than 0";
            mysql_close(conn);
            registered.append(":0");
            return registered;
        }
        else{
            qDebug() << "Row is not greater than 0";
            query = "INSERT INTO Users (NAME, USERNAME, PWORD) VALUES ('" + name + "','" + username + "','" + password + "')";
            query_state = mysql_query(conn, query.toStdString().c_str());
            if (query_state != 0) {
                qDebug() << mysql_error(conn);
                registered.append(":0");
                return registered;
            } else {
                qDebug() << "Data inserted";
            }
            mysql_close(conn);
            registered.append(":1");
            return registered;
        }
    }


}

/**
 * @brief handleMessageThread::handleMessage
 * @param Data
 */
void handleMessageThread::handleMessage(QByteArray Data){
    QStringList read = QString(Data).split(":");

    if(read[0] == "LOGIN"){
        emit ReturnResult(login(read[1],read[2]));
    }
    else if(read[0] == "REGISTER"){
        emit ReturnResult(regAccount(read[1],read[2],read[3]));
    }
    else if(read[0] == "MESSAGE"){
        sendMessage(read);
    }
    else if(read[0] == "CONTACTS"){
        emit ReturnResult(retrieveContacts(read[1]));
    }
    else if(read[0] == "LOGOUT"){
        emit ReturnResult(logout(read[1]));
    }
    else if(read[0] == "SEARCH"){
        emit ReturnResult(searchUsers(read[1], read[2]));
    }
    else if(read[0] == "ADD"){
        emit ReturnResult(addContact(read[1], read[2]));
    }
    else if(read[0] == "RETRIEVEMESSAGES"){
        emit ReturnResult(retrieveMessages(read[1]));
    }
    else if(read[0] == "GROUP"){
        emit ReturnResult(createGroup(read));
    }
    else if(read[0] == "USERONLINE"){
        userOnline(read);
    }
    else if(read[0] == "USEROFFLINE"){

    }
}

/**
 * @brief userOffline
 */
void handleMessageThread::userOffline(QString user){
    conn = mysql_init(&mysql);
    conn = mysql_real_connect(&mysql, server, userName, userPassword,
                              dataBaseName, 0, 0, 0);

    if (conn == NULL) {
        qDebug() << mysql_error(&mysql);
        return;
    } else {
        qDebug() << "Database connection successful.";
    }
    QString query = "SELECT p.ChatID, u.UserID FROM Participants p "
                    "LEFT OUTER JOIN Users u ON u.UserID = p.UserID "
                    "WHERE u.Username = '" + user + "'";
    query_state = mysql_query(conn, query.toStdString().c_str());

    if (query_state != 0) {
        qDebug() << mysql_error(conn);
        return;
    }

    res = mysql_store_result(conn);
    while((row = mysql_fetch_row(res)) != NULL){
        QString SenderID = row[1];
        QString UserID = row[0];
        query = "INSERT INTO Messages (SenderID, ChatID, SendTime, Message) VALUES ('" + SenderID + "','" + UserID + "',UTC_TIMESTAMP,'" + user + " logged out')";
        query_state = mysql_query(conn, query.toStdString().c_str());
    }
}

/**
 * @brief handleMessageThread::userOnline
 */
void handleMessageThread::userOnline(QStringList user){
    conn = mysql_init(&mysql);
    conn = mysql_real_connect(&mysql, server, userName, userPassword,
                              dataBaseName, 0, 0, 0);

    if (conn == NULL) {
        qDebug() << mysql_error(&mysql);
        return;
    } else {
        qDebug() << "Database connection successful.";
    }
    QString query = "SELECT p.ChatID, u.UserID FROM Participants p "
                    "LEFT OUTER JOIN Users u ON u.UserID = p.UserID "
                    "WHERE u.Username = '" + user[1] + "'";
    query_state = mysql_query(conn, query.toStdString().c_str());

    if (query_state != 0) {
        qDebug() << mysql_error(conn);
        return;
    }

    res = mysql_store_result(conn);
    while((row = mysql_fetch_row(res)) != NULL){
        QString SenderID = row[1];
        QString UserID = row[0];
        query = "INSERT INTO Messages (SenderID, ChatID, SendTime, Message) VALUES ('" + SenderID + "','" + UserID + "',UTC_TIMESTAMP,'" + user[1] + " logged in')";
        query_state = mysql_query(conn, query.toStdString().c_str());
    }
}

/**
 * @brief handleMessageThread::createGroup
 * @param participants
 * @return
 */
QStringList handleMessageThread::createGroup(QStringList participants){

    QStringList result;
    result.append("GROUP:");
    participants.pop_front();
    qDebug() << participants;
    participants.sort();

    QString check = "SELECT * FROM Chats WHERE Title = '";
    for(int i = 0; i < participants.length() - 1; i++){
        check.append(participants[i] + "and");
    }
    check.append(participants.last() + "'");
    conn = mysql_init(&mysql);
    conn = mysql_real_connect(&mysql, server, userName, userPassword,
                              dataBaseName, 0, 0, 0);

    if (conn == NULL) {
        qDebug() << mysql_error(&mysql);
        result.append("0");
        return result;
    } else {
        qDebug() << "Database connection successful.";
    }

    query_state = mysql_query(conn, check.toStdString().c_str());

    if (query_state != 0) {
        qDebug() << mysql_error(conn);
        result.append("0");
        return result;
    } else {
        res = mysql_store_result(conn);
        int alreadyExists = mysql_num_rows(res);
        if(alreadyExists > 0){
            result.append("2");
            return result;
        }
    }

    QString query = "INSERT INTO Chats (Title) VALUES ('";
    QString title = "";
    for(int i = 0; i < participants.length() - 1; i++){
        title.append(participants[i] + "and");
    }
    title.append(participants.last());
    query.append(title + "')");

    query_state = mysql_query(conn, query.toStdString().c_str());

    if (query_state != 0) {
        qDebug() << mysql_error(conn);
        result.append("0");
        return result;
    }

    for(int i = 0; i < participants.length(); i++){
        query = "INSERT INTO Participants (ChatID, UserID) VALUES ((SELECT ChatID FROM Chats WHERE Title = '" + title + "')"
                                                                                                                        ", (SELECT UserID FROM Users WHERE Username = '" + participants[i] + "'))";

        query_state = mysql_query(conn, query.toStdString().c_str());

        if (query_state != 0) {
            qDebug() << mysql_error(conn);
            result.append("0");
            return result;
        }
    }

    result.append("1:" + title);
    mysql_close(conn);
    return result;
}

/**
 * @brief handleMessageThread::addContact
 * @param contact
 * @param User
 * @return
 */
QStringList handleMessageThread::addContact(QString contact, QString User){
    QStringList Users;
    Users.append("ADDED");
    conn = mysql_init(&mysql);
    conn = mysql_real_connect(&mysql, server, userName, userPassword,
                              dataBaseName, 0, 0, 0);
    if (conn == NULL) {
        qDebug() << mysql_error(&mysql);
        Users.append(":0");
        return Users;
    } else {
        qDebug() << "Database connection successful.";
    }

    QString check = "SELECT c.ContactUserID FROM Contacts c LEFT OUTER JOIN Users u on c.ContactUserID = u.UserID"
                    " LEFT OUTER JOIN Users b on c.UserID = b.UserID WHERE u.Username = '" + contact + "' AND b.Username = '" + User + "'";
    qDebug() << check;

    query_state = mysql_query(conn, check.toStdString().c_str());

    if (query_state != 0) {
        qDebug() << mysql_error(conn);
        Users.append(":0");
        qDebug() << User;
        return Users;
    } else {
        res = mysql_store_result(conn);
        int alreadyExists = mysql_num_rows(res);
        if(alreadyExists > 0){
            Users.append(":2");
            return Users;
        }
    }

    QStringList alpUsers;
    if(User.toLower() < contact.toLower()){
        alpUsers.append(User);
        alpUsers.append(contact);
    }
    else{
        alpUsers.append(contact);
        alpUsers.append(User);
    }
    QStringList query_list;
    query_list.append ("INSERT INTO Contacts (ContactUserID, UserID) VALUES ((SELECT UserID FROM Users WHERE Username = '" +
                       contact + "'), (SELECT UserID FROM Users WHERE Username = '" + User + "'));");

    query_list.append("INSERT INTO Contacts (ContactUserID, UserID) VALUES ((SELECT UserID FROM Users WHERE Username = '" +
                      User + "'), (SELECT UserID FROM Users WHERE Username = '" + contact + "'));");

    query_list.append("INSERT INTO Chats (Title) VALUES ('" + alpUsers[0] + "and" + alpUsers[1] + "');");

    query_list.append("INSERT INTO Participants (ChatID, UserID) VALUES ((SELECT ChatID FROM Chats WHERE Title = '" +
                      alpUsers[0] + "and" + alpUsers[1] + "'), (SELECT UserID FROM Users WHERE Username = '" +
            User + "'));");

    query_list.append("INSERT INTO Participants (ChatID, UserID) VALUES ((SELECT ChatID FROM Chats WHERE Title = '" +
                      alpUsers[0] + "and" + alpUsers[1] + "'), (SELECT UserID FROM Users WHERE Username = '" +
            contact + "'));");

    for (QString query:query_list){
        qDebug() << query.toStdString().c_str();
        query_state = mysql_query(conn, query.toStdString().c_str());

        if (query_state != 0) {
            qDebug() << query_state;
            qDebug() << mysql_error(conn);

            Users.append(":0");
            qDebug() << User;
            return Users;
        } else {
            qDebug() << "Data inserted";
        }
    }
    mysql_close(conn);
    Users.append(":1");
    return Users;
}

/**
 * @brief handleMessageThread::retrieveContacts
 * @param User
 * @return
 */
QStringList handleMessageThread::retrieveContacts(QString User){

    QStringList contacts;
    contacts.append("CONTACTS");
    conn = mysql_init(&mysql);
    conn = mysql_real_connect(&mysql, server, userName, userPassword,
                              dataBaseName, 0, 0, 0);
    if (conn == NULL) {
        qDebug() << mysql_error(&mysql);
    } else {
        qDebug() << "Database connection successful.";
    }

    QString query = "SELECT Title FROM Chats WHERE Title like '%" + User + "%'; ";
    query_state = mysql_query(conn, query.toStdString().c_str());
    if (query_state != 0) {
        qDebug() << mysql_error(conn);
    } else {
        res = mysql_store_result(conn);
        while((row = mysql_fetch_row(res)) != NULL){
            QString list = row[0];
            contacts.append(":");
            contacts.append(list);
        }
        qDebug() << "Users retrieved: " << contacts;
    }
    mysql_close(conn);
    return contacts;
}

/**
 * @brief handleMessageThread::sendMessage
 * @param MessageInfo
 */
void handleMessageThread::sendMessage(QStringList MessageInfo){

    conn = mysql_init(&mysql);
    conn = mysql_real_connect(&mysql, server, userName, userPassword,
                              dataBaseName, 0, 0, 0);
    if (conn == NULL) {
        qDebug() << mysql_error(&mysql);
        return;
    } else {
        qDebug() << "Database connection successful.";
    }
    QStringList users = MessageInfo[2].split(",");
    for(int i = 0; i < users.length(); i++){
        QString Check = "SELECT SocketID FROM Users WHERE Username = '" + users[i] + "'";
        query_state = mysql_query(conn, Check.toStdString().c_str());
        if (query_state != 0) {
            qDebug() << mysql_error(conn);
            return;
        } else {

            qDebug() << "User online";
            res = mysql_store_result(conn);
            row = mysql_fetch_row(res);
            QString data = row[0];
            if(data == "-1"){
                continue;
            }
            else{
                QStringList userOnline;
                userOnline.append("INCOMINGMESSAGE:");
                userOnline.append(row[0]);
                userOnline.append(":");
                userOnline.append(MessageInfo[1]);
                userOnline.append(":");
                userOnline.append(MessageInfo[2]);
                userOnline.append(":");
                userOnline.append(MessageInfo[3]);
                emit(ReturnMessage(userOnline));
            }

        }
    }


    QStringList alpUsers;
    if(MessageInfo[1].toLower() < MessageInfo[2].toLower()){
        alpUsers.append(MessageInfo[1]);
        alpUsers.append(MessageInfo[2]);
    }
    else{
        alpUsers.append(MessageInfo[2]);
        alpUsers.append(MessageInfo[1]);
    }
    users.append(MessageInfo[1]);
    users.sort();
    QString chat = "";
    for(int i = 0; i < users.length() - 1; i++){
        chat.append(users[i]);
        chat.append("and");
    }
    chat.append(users.last());
    QString query = "INSERT INTO Messages (SenderID, ChatID, SendTime, Message) VALUES ((SELECT UserID FROM Users WHERE Username = '"
            + MessageInfo[1] + "'), (SELECT ChatID FROM Chats WHERE Title = '"  + chat + "'), UTC_TIMESTAMP, '" + MessageInfo[3] + "')";
    qDebug() << query;
    query_state = mysql_query(conn, query.toStdString().c_str());

    if (query_state != 0) {
        qDebug() << mysql_error(conn);
        return;
    } else {
        qDebug() << "Data inserted";
    }
    mysql_close(conn);
}

/**
 * @brief handleMessageThread::searchUsers
 * @param user
 * @param criteria
 * @return
 */
QStringList handleMessageThread::searchUsers(QString user, QString criteria){
    QStringList users;
    users.append("USERS");
    conn = mysql_init(&mysql);
    conn = mysql_real_connect(&mysql, server, userName, userPassword,
                              dataBaseName, 0, 0, 0);
    if (conn == NULL) {
        qDebug() << mysql_error(&mysql);
    } else {
        qDebug() << "Database connection successful.";
    }

    QString query = "SELECT Name, Username FROM Users WHERE Username LIKE '%" + criteria + "%'";
    query_state = mysql_query(conn, query.toStdString().c_str());
    if (query_state != 0) {
        qDebug() << mysql_error(conn);
    } else {
        qDebug() << ":Users retrieved" << users;
    }
    res = mysql_store_result(conn);
    mysql_close(conn);
    int rows = mysql_num_rows(res);
    if(rows == 0){
        users.append(":NO USERS RETURNED");
        return users;
    }

    while((row = mysql_fetch_row(res)) != NULL){
        if(row[1] == user){continue;}
        users.append(":");
        users.append(row[0]);
        users.append(":");
        users.append(row[1]);
    }
    return users;
}

/**
 * @brief handleMessageThread::retrieveMessages
 * @param chatName
 * @return
 */
QStringList handleMessageThread::retrieveMessages(QString chatName){
    QStringList messages;
    messages.append("MESSAGE");
    conn = mysql_init(&mysql);
    conn = mysql_real_connect(&mysql, server, userName, userPassword,
                              dataBaseName, 0, 0, 0);
    if (conn == NULL) {
        qDebug() << mysql_error(&mysql);
    } else {
        qDebug() << "Database connection successful.";
    }

    QString query = "SELECT m.Message, m.SenderID, u.Username From Messages m LEFT OUTER JOIN Chats c ON c.ChatID = m.ChatID "
                    "LEFT OUTER JOIN Users u ON u.UserID = m.SenderID WHERE c.Title = '" + chatName + "' ORDER BY m.SendTime";

    query_state = mysql_query(conn, query.toStdString().c_str());
    if (query_state != 0) {
        qDebug() << mysql_error(conn);
    } else {
        qDebug() << "Messages retrieved: " << messages;
    }
    res = mysql_store_result(conn);
    mysql_close(conn);

    while((row = mysql_fetch_row(res)) != NULL){
        messages.append(":");
        messages.append(row[0]);
        messages.append(":");
        messages.append(row[1]);
        messages.append(":");
        messages.append(row[2]);
    }
    return messages;
}





















