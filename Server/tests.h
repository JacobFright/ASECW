#ifndef TESTS_H
#define TESTS_H


#include <QObject>
#include <QtTest/QtTest>
#include "handlemessagethread.h"

class tests : public QObject
{
    Q_OBJECT
public:
    explicit tests(QObject *parent = nullptr);

signals:

private slots:
    void login();
    void registration();

private:
    handleMessageThread *handleMessage;
};

#endif // TESTS_H
