#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QHostAddress>
#include <QTimer>

class TcpClient : public QObject
{
    Q_OBJECT
public:

    explicit TcpClient(QObject *parent = nullptr);

    void connectToServer();
    void disconnectFromServer();


private slots:

    void handleConnectedToServer();

    void handleDisconnectedFromServer();

    void handleReadyRead();

    void handleSocketError(QAbstractSocket::SocketError sockErr);

    void handleOneSecTimerTriggered();
    void handleBigTimerTriggered();
private:

    QHostAddress _serverIp      = QHostAddress("172.16.25.15");
    quint16      _serverPort    = 6000;

    quint32  _oneSecTimerDataCounter    = 0;
    quint32  _bigTimerDataCounter       = 0;

    QTcpSocket* _socket     = nullptr;

    QTimer * _bigTimer      = nullptr;

    quint32 _bigTimerPeriod_ms = 30000;
};

#endif // TCPCLIENT_H
