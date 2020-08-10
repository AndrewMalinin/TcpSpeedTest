#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QHostAddress>
#include <QTimer>
#include <QDebug>
#include <QProcess>


#define PLATFORM ( 1 )  // 1 = "linux" or  2 = "windows"
#define PACKET_HEADER_LENGTH (6); //bytes

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

    QHostAddress _serverIp      = QHostAddress(QString("172.16.25.10"));
    quint16      _serverPort    = 6000;

    qint32  _oneSecTimerDataCounter    = 0;
    qint32  _bigTimerDataCounter       = 0;
    qint32  _maxSpeed                  = 0;

    qint32 _lastOneSecTimerSpeed       = 0;
    qint32 _lastBigTimerSpeed          = 0;

    QTcpSocket* _socket     = nullptr;

    QTimer*  _bigTimer      = nullptr;

    quint32 _bigTimerPeriod_ms = 10000;
};

#endif // TCPCLIENT_H
