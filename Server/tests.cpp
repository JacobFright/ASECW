#include "tests.h"

tests::tests(QObject *parent) : QObject(parent)
{
    QByteArray test;
    QTcpSocket tests;
    //handleMessageThread *handleMessage = new handleMessageThread(tests,test,this);
}

void tests::login(){
    //QVERIFY(handleMessage->login("Test","123") == true);
    //QVERIFY(handleMessage->login("Test","123\n") == true);
    //QVERIFY(handleMessage->login("Test","test") == false);
}

void tests::registration(){
//    QVERIFY(handleMessage->regAccount("Jacob","Test", "123") == false);
//    QVERIFY(handleMessage->regAccount("Jacob","Test", "321") == false);
    //QVERIFY(handleMessage->regAccount("Bob","Greg", "123") == true);
}


//QTEST_MAIN(tests)
