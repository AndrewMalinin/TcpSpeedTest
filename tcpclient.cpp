#include "tcpclient.h"

TcpClient::TcpClient(QObject *parent) : QObject(parent)
{

    _socket = new QTcpSocket(this);

    if(nullptr != _socket)
    {
        connectToServer();
    }
}

TcpClient::~TcpClient()
{
    delete _socket;
    delete _bigTimer;
}

void TcpClient::connectToServer()
{
    if(nullptr == _socket)
    {
        return;
    }

    // Если сокет уже подключен или находится в процессе установки соединения
    if (QAbstractSocket::ConnectedState  == _socket->state() ||
            QAbstractSocket::ConnectingState == _socket->state())
    {
        return;
    }
    qDebug() << "> Connecting...";
    _socket->connectToHost(_serverIp, _serverPort);
    connect(_socket, &QTcpSocket::connected,
            this, &TcpClient::handleConnectedToServer);
    connect(_socket, &QTcpSocket::readyRead,
            this, &TcpClient::handleReadyRead);
    connect(_socket, &QTcpSocket::disconnected,
            this, &TcpClient::handleDisconnectedFromServer);
    connect(_socket, QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::error),
            this, &TcpClient::handleSocketError);
}

void TcpClient::disconnectFromServer()
{
    if(nullptr == _socket)
    {
        _socket->disconnectFromHost();
        // Обрываем связи сигналов и слотов
        _socket->disconnect();
    }
}

void TcpClient::handleConnectedToServer()
{   qDebug() << "> Connected!";
    _bigTimer->setInterval(_bigTimerPeriod_ms);
    connect(_bigTimer, &QTimer::timeout, this, &TcpClient::handleBigTimerTriggered);
    _bigTimer->start();
    QTimer::singleShot(1000, this, &TcpClient::handleOneSecTimerTriggered);
}

void TcpClient::handleDisconnectedFromServer()
{
    qDebug() << "> Disconnect from server!";
    _socket->disconnect();
    _bigTimer->disconnect();
}

void TcpClient::handleSocketError(QAbstractSocket::SocketError sockErr)
{
    qDebug() << "> Connection error!:" << sockErr;
    handleDisconnectedFromServer();
}


void TcpClient::handleReadyRead()
{
    if(nullptr == _socket)
    {
        return;
    }

    QByteArray receivedData = _socket->readAll();
    quint32 receivedDataLength = receivedData.length();

    _oneSecTimerDataCounter += receivedDataLength;
    _bigTimerDataCounter    += receivedDataLength;
}

void TcpClient::handleOneSecTimerTriggered()
{
    _lastOneSecTimerSpeed = _oneSecTimerDataCounter;
    _oneSecTimerDataCounter = 0;
    viewUpdate();
    if (QAbstractSocket::ConnectedState  == _socket->state())
        QTimer::singleShot(1000, this, &TcpClient::handleOneSecTimerTriggered);
}

void TcpClient::handleBigTimerTriggered()
{
    viewUpdate();
}

void TcpClient::viewUpdate()
{
    if      (PLATFORM  == 2)
        QProcess::execute("cmd /c cls");
    else if (PLATFORM  == 1)
        QProcess::execute("clear");
    qDebug().noquote() << QString("Speed per second: %1;  Mean Speed per second: %2").arg(_lastOneSecTimerSpeed).arg(_lastBigTimerSpeed);
}
