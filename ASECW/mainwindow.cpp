#include "mainwindow.h"
#include "ui_mainwindow.h"
/**
 * @brief MainWindow::MainWindow
 * @param lsocket
 * @param user
 * @param UserID
 * @param parent
 * Contructor for the MainWindow class.
 */
MainWindow::MainWindow(QTcpSocket *lsocket, QString user, QString UserID, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tabPages->setCurrentIndex(0);
    ui->tblResults->setColumnWidth(0,ui->tabPages->width()/3.1);
    ui->tblResults->setColumnWidth(1,ui->tabPages->width()/3.1);
    ui->tblResults->setColumnWidth(2,ui->tabPages->width()/3.1);
    this->user = user;
    this->UserID = UserID;
    socket = lsocket;
    connect(socket,SIGNAL(connected()),this,SLOT(connected()));
    connect(socket,SIGNAL(disconnected()),this,SLOT(disconnected()));
    retrieveContacts(user);
}

/**
 * @brief MainWindow::~MainWindow
 * Deconstructor for the MainWindow Class
 */
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setOnline(){
    QTextStream os(socket);
    os << "USERONLINE:" + user;
}

/**
 * @brief MainWindow::on_btnLogout_clicked
 * Sends the request to the server to log the user out.
 */
void MainWindow::on_btnLogout_clicked()
{
    QTextStream os(socket);
    os << "LOGOUT:" + user;
}

/**
 * @brief MainWindow::on_btnSend_clicked
 * Adds the message to the users message window,
 * sends the server a request to pass the message over to the selected recipient.
 */
void MainWindow::on_btnSend_clicked()
{
    ui->lstMsgHistory->addItem(ui->txtMsg->text());

    QString recipients = ui->lstContact->currentItem()->text();
    QStringList recip = recipients.split(",");
    recip.append(user);
    recip.sort();
    int stop = recip.length();
    for(int i = 1; i < recip.length(); i+=2){
        recip.insert(i,"and");
    }
    QTextStream os(socket);
    qDebug() << "MESSAGE:" + user + ":"+ recipients + ":" + ui->txtMsg->text();
    os << "MESSAGE:" + user + ":"+ recipients + ":" + ui->txtMsg->text();
    ui->txtMsg->setText("");
    ui->lstMsgHistory->scrollToBottom();
}

///**
// * @brief MainWindow::connected
// *
// */
void MainWindow::connected(){
    qDebug() << "Client Connected Event";
}

///**
// * @brief MainWindow::disconnected
// */
void MainWindow::disconnected(){
    qDebug() << "Client Disconnected from socket " + QString::number(socket->socketDescriptor());
    socket->deleteLater();
    exit(0);
}

/**
 * @brief MainWindow::retrieveContacts
 * @param user
 */
void MainWindow::retrieveContacts(QString user){
    QTextStream os(socket);
    qDebug() << "Retrieve contacts from socket " + QString::number(socket->socketDescriptor());
    os << "CONTACTS:" + user;
}

/**
 * @brief MainWindow::handleServerResponse
 * @param Result
 */
void MainWindow::handleServerResponse(QByteArray Result){
    QStringList Response = QString(Result).split(":");
    if(Response[0] == "CONTACTS"){
        ui->lstContact->clear();
        for(int i = 1; i < Response.length(); i++){
            Response[i].replace("and",",");
            Response[i].remove(user);
            if(Response[i].endsWith(",")){
                Response[i].chop(1);
            }
            if(Response[i].startsWith(",")){
                Response[i].remove(0,1);
            }
            Response[i].replace(",,",",");
            QListWidgetItem *item = new QListWidgetItem(Response[i]);
            ui->lstContact->addItem(item);
        }
            setOnline();
    }
    else if(Response[0] == "LOGOUT"){
        if(Response[1] == "1"){
            this->close();
        }
        else{
            QMessageBox::information(this, "Logout", "Logout failed");
            return;
        }
    }
    else if(Response[0] == "USERS"){
        ui->txtSearch->setText("");
        ui->tblResults->clearContents();
        ui->tblResults->setRowCount(0);
        for(int i = 1; i < Response.length(); i+=2){
            ui->tblResults->insertRow(ui->tblResults->rowCount());
            ui->tblResults->setItem(ui->tblResults->rowCount()-1, 1, new QTableWidgetItem(Response[i]));
            ui->tblResults->setItem(ui->tblResults->rowCount()-1, 2, new QTableWidgetItem(Response[i+1]));
        }
    }
    else if(Response[0] == "ADDED"){
        if(Response[1] == "0"){
            QMessageBox::information(this,"Add Contact", "Failed to add contact");
        }
        else if(Response[1] == "1"){
            retrieveContacts(user);
        }
        else{
            QMessageBox::information(this,"Add Contact","User is already in your contacts");
        }
    }
    else if(Response[0] == "MESSAGE"){
        ui->lstMsgHistory->clear();
        for(int i = 1; i < Response.length(); i+=3){
            if(Response[i+1] == UserID){
                ui->lstMsgHistory->addItem(Response[i]);
            }
            else{
                QListWidgetItem *replies = new QListWidgetItem(Response[i+2] + ": " + Response[i]);
                replies->setTextAlignment(Qt::AlignRight);
                ui->lstMsgHistory->addItem(replies);
            }
            ui->lstMsgHistory->scrollToBottom();
        }
    }
    else if(Response[0] == "INCOMINGMESSAGE"){
        int test = ui->tabPages->currentIndex();
        QString chat = Response[3].replace(user,Response[2]);
        QStringList tmp = chat.split(",");
        tmp.sort();
        chat = tmp.join(",");

        if(ui->lstContact->currentItem() == NULL){
            for(int i = 1; i < ui->lstContact->count(); i++){
                if(ui->lstContact->itemAt(i,0)->text() == Response[3]){
                    ui->lstContact->itemAt(i,0)->setBackgroundColor("#0000FF");
                }
            }
        }
        else if(ui->lstContact->currentItem()->text() == chat && test == 0){
            QListWidgetItem *test = ui->lstContact->currentItem();
            test->setBackgroundColor("#FFFFFF");
            QTextStream os(socket);
            QListWidgetItem *item = ui->lstContact->currentItem();
            QStringList chatName;
            chatName.append(user);
            QStringList names = item->text().split(",");
            for(int i = 0; i < names.length(); i++){
                chatName.append(names[i]);
            }
            chatName.sort();
            QString chat = chatName[0];
            for(int i = 1; i < chatName.length(); i++){
                chat.append("and" + chatName[i]);
            }
            chat.replace(",","and");
            os << "RETRIEVEMESSAGES:" + chat;
            //            QListWidgetItem *newMessage = new QListWidgetItem(Response[3]);
            //            newMessage->setTextAlignment(Qt::AlignRight);
            //            ui->lstMsgHistory->addItem(newMessage);
        }
        else{
            for(int i = 1; i < ui->lstContact->count(); i++){
                if(ui->lstContact->itemAt(i,0)->text() == Response[3]){
                    ui->lstContact->itemAt(i,0)->setBackgroundColor("#0000FF");
                }
            }
        }
    }
    else if(Response[0] == "GROUP"){
        if(Response[1] == "2"){
            QMessageBox::information(this, "Create Group", "Group already exists");
        }
        else if(Response[1] == 0){
            QMessageBox::information(this, "Create Group", "Failed to create group");
        }
        else{
            retrieveContacts(user);
        }
    }
    else if(Response[0] == "USERONLINE"){
        for(int i = 1; i < ui->lstContact->count(); i++){
            if(ui->lstContact->itemAt(i,0)->text() == Response[1]){
                ui->lstContact->itemAt(i,0)->setBackgroundColor("#00FF00");
            }
        }
    }
}

/**
 * @brief MainWindow::on_btnAddContact_clicked
 */
void MainWindow::on_btnAddContact_clicked()
{
    QModelIndex index = ui->tblResults->selectionModel()->currentIndex();
    QVariant value = index.sibling(index.row(),2).data();
    QTextStream os(socket);
    os << "ADD:" + value.toString() + ":" + user;
}

/**
 * @brief MainWindow::on_btnSearch_clicked
 */
void MainWindow::on_btnSearch_clicked()
{
    ui->tabPages->setCurrentIndex(3);
    QTextStream os(socket);
    os << "SEARCH:" + user + ":" + ui->txtSearch->text();
}

/**
 * @brief MainWindow::on_lstContact_itemClicked
 * @param item
 * Moves the main window onto the message screen,
 * requests the server to retrieve the messages associated with the selected chat.
 */
void MainWindow::on_lstContact_itemClicked(QListWidgetItem *item)
{
    ui->tabPages->setCurrentIndex(0);
    item->setBackgroundColor("#FFFFFF");
    QTextStream os(socket);
    QStringList chatName;
    chatName.append(user);
    QStringList names = item->text().split(",");
    for(int i = 0; i < names.length(); i++){
        chatName.append(names[i]);
    }
    chatName.sort();
    QString chat = chatName[0];
    for(int i = 1; i < chatName.length(); i++){
        chat.append("and" + chatName[i]);
    }

    os << "RETRIEVEMESSAGES:" + chat;
    qDebug() << item->text();
    //os << "RETRIEVEMESSAGES:" + user + ":" + item->text();
    //ui->lstMsgHistory->scrollToBottom();
}

/**
 * @brief MainWindow::on_btnGroup_clicked
 * Takes the selected users from the contact list,
 * checks if it contains an existing group,
 * requests the server create the group chat containing those members.
 */
void MainWindow::on_btnGroup_clicked()
{
    QStringList check;
    QString recipients = "GROUP:" + user;
    if(ui->lstContact->selectedItems().count() == 1){
        QMessageBox::information(this,"Create Group Chat","Only one user selected");
        return;
    }
    foreach (QListWidgetItem *item, ui->lstContact->selectedItems()) {
        QStringList check = item->text().split(",");
        if(check.length() > 1){
            QMessageBox::information(this,"Create Group Chat","Cannot create chat with from existing chat");
            return;
        }
        recipients.append(":" + item->text());
    }
    QTextStream os(socket);
    os << recipients;
}






