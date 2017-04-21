#include "websocketwrapper.h"
#include "logger.h"

WebSocketWrapper::WebSocketWrapper(QWebSocket *socket, QMutex* mutex,QObject *parent)
    : QObject(parent), m_pSocket(socket),m_pMutex(mutex)
{
    TRACE("Server", "< WebSocketWrapper > created.");

    connect(socket, &QWebSocket::connected, this, &WebSocketWrapper::socketConnected);
    connect(socket, &QWebSocket::disconnected, this, &WebSocketWrapper::socketDisconnected);

    // check if socket is connected at first start
    m_connected = socket->isValid();
}

WebSocketWrapper::~WebSocketWrapper()
{
    TRACE("Server", "< WebSocketWrapper > destroyed.");
}

qint64 WebSocketWrapper::sendTextMessage(const QString &message)
{
    QMutexLocker locker(m_pMutex); // locking per client
    qint64 bytesSent = 0;

    if (m_connected)
    {
        TRACE("Server", "Sending message : " + message);
        bytesSent = m_pSocket->sendTextMessage(message);

        // flush seems cause problems, why?
        //if (m_pSocket->)
        //{
            m_pSocket->flush(); // well, sometimes (seen in iOS) you really need to flush
        //}

        TRACE("Server", "Bytes sent: " + QString::number(bytesSent));
    }
    else
    {
        WARNING("Server", "Socket not connected. Unable to send.");
        TRACE("Server", "Message : " + message);
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
