#include <QCoreApplication>
#include "tcpclient.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    qDebug() << "> Client starting...";
    TcpClient* tcpClient = new TcpClient();
    qDebug() << "> Client is started!";
    return a.exec();
}
