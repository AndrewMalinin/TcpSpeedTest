#include <QCoreApplication>
#include <QDebug>
#include <QProcess>
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QProcess::execute("clear");
    return a.exec();
}
