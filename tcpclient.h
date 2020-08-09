#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QHostAddress>
#include <QTimer>
#include <QDebug>
#include <QProcess>


#define PLATFORM ( 2 )  // 1 = "linux" or  2 = "windows"

class TcpClient : public QObject
{
    Q_OBJECT
public:

    explicit TcpClient(QObject *parent = nullptr);
    virtual ~TcpClient();

    void connectToServer();
    void disconnectFromServer();
    void viewUpdate();

private slots:

    void handleConnectedToServer();

    void handleDisconnectedFromServer();

    void handleReadyRead();

    void handleSocketError(QAbstractSocket::SocketError sockErr);

    void handleOneSecTimerTriggered();
    void handleBigTimerTriggered();
private:

    QHostAddress _serverIp      = QHostAddress(QString("127.0.0.1"));
    quint16      _serverPort    = 6000;

    quint32  _oneSecTimerDataCounter    = 0;
    quint32  _bigTimerDataCounter       = 0;

    quint32 _lastOneSecTimerSpeed       = -1;
    quint32 _lastBigTimerSpeed          = -1;

    QTcpSocket* _socket     = nullptr;

    QTimer*  _bigTimer      = nullptr;

    quint32 _bigTimerPeriod_ms = 2000;
};

#endif // TCPCLIENT_H
