#include "websocketwrapper.h"


WebSocketWrapper::WebSocketWrapper(QWebSocket *socket, QMutex* mutex,QObject *parent)
    : QObject(parent), m_pSocket(socket),m_pMutex(mutex)
{
    //qDebug() << "WebSocketWrapper created.";

    connect(socket, &QWebSocket::connected, this, &WebSocketWrapper::socketConnected);
    connect(socket, &QWebSocket::disconnected, this, &WebSocketWrapper::socketDisconnected);

    // check if socket is connected at first start
    m_connected = socket->isValid();
}

WebSocketWrapper::~WebSocketWrapper()
{

    //qDebug() << "WebSocketWrapper is dying...";
}

qint64 WebSocketWrapper::sendTextMessage(const QString &message)
{
    QMutexLocker locker(m_pMutex); // locking per client
    qint64 bytesSent = 0;

    if (m_connected)
    {
        qDebug() << "Sending: " << message ;
        bytesSent = m_pSocket->sendTextMessage(message);
        m_pSocket->flush(); // well, sometimes you really need to flush
        qDebug() << "Sending: byte sent " << bytesSent;
    }
    else
    {
        qWarning() << "SOCKET NOT CONNECTED!" << message;
        qDebug() << "Message: " << message;
    }

    m_lastMessage = message;

    return bytesSent;
}

const QString WebSocketWrapper::getLastMessage()
{
    return m_lastMessage;
}

void WebSocketWrapper::socketDisconnected()
{
    m_connected = false;
}

void WebSocketWrapper::socketConnected()
{
    m_connected = true;
}

QWebSocket *WebSocketWrapper::getSocket() const
{
    return m_pSocket;
}
