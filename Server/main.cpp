#include "myserver.h"
#include <QApplication>

/**
 * @brief main
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MyServer server;
    server.StartServer();
    return a.exec();
}
