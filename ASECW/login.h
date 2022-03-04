#ifndef LOGIN_H
#define LOGIN_H

#include <QMainWindow>
#include "registration.h"
#include "mainwindow.h"
#include <QTcpSocket>

QT_BEGIN_NAMESPACE
namespace Ui { class LogIn; }
QT_END_NAMESPACE

class LogIn : public QMainWindow
{
    Q_OBJECT

public:
    LogIn(QWidget *parent = 0);
    void sendToServer(QString);
    void handleServerResponse(QByteArray);

    ~LogIn();

public slots:
    void connected();
    void readyRead();
    void disconnected();

private slots:
    void on_btnReg_clicked();
    void on_btnLogin_clicked();
    void on_btnCancel_clicked();

private:
    Ui::LogIn *ui;
    Registration *reg;
    MainWindow *home;
    QTcpSocket *socket;
    QString encryptPword(QString);
};
#endif // LOGIN_H
