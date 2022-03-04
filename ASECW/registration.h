#ifndef REGISTRATION_H
#define REGISTRATION_H

#include <QDialog>
#include <QTcpSocket>
#include <QMessageBox>

namespace Ui {
class Registration;
}

class Registration : public QDialog
{
    Q_OBJECT

public:
    explicit Registration(QTcpSocket *socket, QWidget *parent = nullptr);
    void sendToServer(QString message);
    void handleServerResponse(QByteArray Response);
    ~Registration();

public slots:
    void connected();
    void disconnected();

private slots:
    void on_btnCancel_clicked();

    void on_btnReg_clicked();

private:
    Ui::Registration *ui;
    QTcpSocket *socket;
    QString encryptPword(QString);
};

#endif // REGISTRATION_H
