#include "tcpclient.h"

TcpClient::TcpClient(QObject *parent) : QObject(parent)
{

    _socket = new QTcpSocket(this);
    _bigTimer = new QTimer(this);
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
    quint32 receivedDataLength = receivedData.size() - PACKET_HEADER_LENGTH;

    _oneSecTimerDataCounter += receivedDataLength;
    _bigTimerDataCounter    += receivedDataLength;
}

void TcpClient::handleOneSecTimerTriggered()
{
    _lastOneSecTimerSpeed = _oneSecTimerDataCounter;

    if(_maxSpeed < _oneSecTimerDataCounter)
        _maxSpeed = _oneSecTimerDataCounter;

    _oneSecTimerDataCounter = 0;
    viewUpdate();
    if (QAbstractSocket::ConnectedState  == _socket->state())
        QTimer::singleShot(1000, this, &TcpClient::handleOneSecTimerTriggered);
}

void TcpClient::handleBigTimerTriggered()
{
    _lastBigTimerSpeed = _bigTimerDataCounter / (_bigTimerPeriod_ms/1000.0);

    if(_maxSpeed < _lastBigTimerSpeed)
        _maxSpeed = _lastBigTimerSpeed;

    _bigTimerDataCounter = 0;
    viewUpdate();
}

void TcpClient::viewUpdate()
{
    if      (PLATFORM  == 2)
        QProcess::execute("cmd /c cls");
    else if (PLATFORM  == 1)
        QProcess::execute("clear");

    qDebug().noquote() << QString("Data channel Width:\nInstant:\t %1\tKBits/s \nMean:\t\t %2\tKBits/s \n"
                                  "Max Speed:\t %3\tKBits/s")
                          .arg(_lastOneSecTimerSpeed*8/1024.0)
                          .arg(_lastBigTimerSpeed*8/1024.0)
                          .arg( QString::number(8*(_maxSpeed/1024.0)));
}
