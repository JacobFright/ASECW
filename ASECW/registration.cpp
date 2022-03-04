#include "registration.h"
#include "ui_registration.h"

/**
 * @brief Registration::Registration
 * @param lsocket
 * @param parent
 */
Registration::Registration(QTcpSocket *lsocket, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Registration)
{
    ui->setupUi(this);
    socket = lsocket;
    connect(socket,SIGNAL(connected()),this,SLOT(connected()));
    connect(socket,SIGNAL(disconnected()),this,SLOT(disconnected()));
}

/**
 * @brief Registration::~Registration
 */
Registration::~Registration()
{
    delete ui;
}

/**
 * @brief Registration::on_btnCancel_clicked
 */
void Registration::on_btnCancel_clicked()
{
    this->close();
}

/**
 * @brief Registration::sendToServer
 * @param message
 */
void Registration::sendToServer(QString message)
{
    // write to the server
    QTextStream os(socket);
    os << message;

}

/**
 * @brief Registration::on_btnReg_clicked
 */
void Registration::on_btnReg_clicked()
{
    if(ui->txtPwd->text() != ui->txtCfmPwd->text())
    {
        QMessageBox::information(this,"Register","The passwords entered do not match");
        ui->txtCfmPwd->setText("");
        return;
    }
    QString regDetails = "REGISTER:" + ui->txtName->text() + ":" + ui->txtUser->text() + ":" + encryptPword(ui->txtPwd->text());
    sendToServer(regDetails);
}

/**
 * @brief Registration::connected
 */
void Registration::connected(){
    qDebug() << "Client Connected Event";
}

/**
 * @brief Registration::disconnected
 */
void Registration::disconnected(){
    qDebug() << "Client Disconnected";
    socket->deleteLater();
    exit(0);
}

/**
 * @brief Registration::handleServerResponse
 * @param Response
 */
void Registration::handleServerResponse(QByteArray Response){
    QStringList read = QString(Response).split(":");
    qDebug() << "Register Result:" << Response;

    if(read[1] == "1"){
        QMessageBox::information(this,"Register","Account Registered");
        this->close();
    }
    else{
        QMessageBox::information(this,"Register","Username in use");
        return;
    }
}

/**
 * @brief Registration::encryptPword
 * @param PWord
 * @return
 */
QString Registration::encryptPword(QString PWord){
    std::string Password = PWord.toStdString();
    for(int i = 0; i < PWord.length(); i++){
        Password[i] = Password[1] + 2;
    }
    return QString::fromStdString(Password);
}
