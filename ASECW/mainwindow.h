#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QMessageBox>
#include <QListWidget>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QTcpSocket *socket, QString user, QString UserID, QWidget *parent = nullptr);
    void retrieveContacts(QString user);
    void handleServerResponse(QByteArray Result);
    void setOnline();
    ~MainWindow();

public slots:
    void connected();
    void disconnected();

private slots:
    void on_btnLogout_clicked();
    void on_btnSend_clicked();
    void on_btnAddContact_clicked();
    void on_btnSearch_clicked();
    void on_lstContact_itemClicked(QListWidgetItem *item);
    void on_btnGroup_clicked();

private:
    Ui::MainWindow *ui;
    QTcpSocket *socket;
    QString user, UserID;
};

#endif // MAINWINDOW_H
