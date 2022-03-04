#include "login.h"
#include "ui_login.h"

/**
 * @brief LogIn::LogIn
 * @param parent
 */
LogIn::LogIn(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::LogIn)
{
    ui->setupUi(this);

    socket = new QTcpSocket(this);
    connect(socket,SIGNAL(connected()),this,SLOT(connected()));
    connect(socket,SIGNAL(disconnected()),this,SLOT(disconnected()));
    connect(socket,SIGNAL(readyRead()),this,SLOT(readyRead()));
    socket->connectToHost("localhost", 8080);
}

/**
 * @brief LogIn::~LogIn
 */
LogIn::~LogIn()
{
    delete ui;
}

/**
 * @brief LogIn::connected
 */
void LogIn::connected(){
    qDebug() << "Client Connected on socket " + QString::number(socket->socketDescriptor());
}

/**
 * @brief LogIn::disconnected
 */
void LogIn::disconnected(){
    qDebug() << "Client Disconnected from socket " + QString::number(socket->socketDescriptor());
    socket->deleteLater();
    exit(0);
}

/**
 * @brief LogIn::readyRead
 */
void LogIn::readyRead(){
    QByteArray Data = socket->readAll();
    qDebug() << " Data in: " << Data;
    handleServerResponse(Data);
}

/**
 * @brief LogIn::on_btnReg_clicked
 */
void LogIn::on_btnReg_clicked()
{
    reg = new Registration(socket, this);
    reg->show();
}

/**
 * @brief LogIn::on_btnLogin_clicked
 */
void LogIn::on_btnLogin_clicked()
{
    QString message = "LOGIN:" + ui->txtUser->text() + ":" + encryptPword(ui->txtPwd->text());
    sendToServer(message);

}

/**
 * @brief LogIn::encryptPword
 * @param PWord
 * @return
 */
QString LogIn::encryptPword(QString PWord){
    std::string Password = PWord.toStdString();
    for(int i = 0; i < PWord.length(); i++){
        Password[i] = Password[1] + 2;
    }
    return QString::fromStdString(Password);
}

/**
 * @brief LogIn::on_btnCancel_clicked
 */
void LogIn::on_btnCancel_clicked()
{
    this->close();
}

/**
 * @brief LogIn::sendToServer
 * @param message
 */
void LogIn::sendToServer(QString message)
{
    // write to the server
    QTextStream os(socket);
    os << message;
}

/**
 * @brief LogIn::handleServerResponse
 * @param Response
 */
void LogIn::handleServerResponse(QByteArray Response){
    QStringList read = QString(Response).split(":");
    qDebug() << "Server Response: : " << Response;

    if(read[0] == "LOGIN"){
        if(read[1] == "1"){
            home = new MainWindow(socket, ui->txtUser->text(), read[2]);
            home->show();
            this->close();
        }
        else{
            QMessageBox::information(this,"Login","Incorrect Password");
            ui->txtPwd->setText("");
            return;
        }
    }
    else if(read[0] == "REGISTER"){
        reg->handleServerResponse(Response);
    }
    else{
        home->handleServerResponse(Response);
    }
//    else if(read[0] == "CONTACTS"){
//        home->handleServerResponse(Response);
//    }
//    else if(read[0] == "LOGOUT"){
//        home->handleServerResponse(Response);
//    }
//    else if(read[0] == "USERS"){
//        home->handleServerResponse(Response);
//    }
//    else if(read[0] == "ADDED"){
//        home->handleServerResponse(Response);
//    }
//    else if(read[0] == "MESSAGE"){
//        home->handleServerResponse(Response);
//    }
//    else if(read[0] == "INCOMINGMESSAGE"){
//        home->handleServerResponse(Response);
//    }

}

























