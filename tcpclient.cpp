#include "tcpclient.h"

TcpClient::TcpClient(QObject *parent) : QObject(parent)
{

    _socket = new QTcpSocket(this);

    _oneSecTimer->setInterval(1000);
    _bigTimer->setInterval(_bigTimerPeriod_ms);
    connect(_oneSecTimer, &QTimer::timeout, this, &TcpClient::handleOneSecTimerTriggered);

    if(nullptr != _socket)
    {
        connectToServer();
    }
}

void TcpClient::connectToServer()
{
    if(nullptr != _socket)
    {
        return;
    }

    // Если сокет уже подключен или находится в процессе установки соединения
    if (QAbstractSocket::ConnectedState  == _socket->state() ||
            QAbstractSocket::ConnectingState == _socket->state())
    {
        return;
    }

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
    if(nullptr != _socket)
    {
        _socket->disconnectFromHost();
        // Обрываем связи сигналов и слотов
        _socket->disconnect();
    }
}

void TcpClient::handleReadyRead()
{
    if(nullptr != _socket)
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


    _lastOneSecTimerSpeed =
    _oneSecTimerDataCounter = 0;
    viewUpdate();
}
