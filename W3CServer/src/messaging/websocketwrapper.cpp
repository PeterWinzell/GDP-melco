#include "websocketwrapper.h"


WebSocketWrapper::WebSocketWrapper(QWebSocket *socket, QMutex* mutex,QObject *parent)
    : QObject(parent), m_pSocket(socket),m_pMutex(mutex)
{
    qDebug() << "WebSocketWrapper created.";
}

WebSocketWrapper::~WebSocketWrapper()
{
    qDebug() << "WebSocketWrapper is dying...";
}

qint64 WebSocketWrapper::sendTextMessage(const QString &message)
{
    QMutexLocker locker(m_pMutex); // locking per client
    qint64 bytesSent = 0;

    if (m_pSocket != 0 && m_pSocket->isValid())
    {
        qDebug() << "Sending: " << message;
        bytesSent = m_pSocket->sendTextMessage(message);
    }
    else
    {
        qWarning() << "Socket not connected! Message: " << message;
    }

    m_lastMessage = message;

    return bytesSent;
}

const QString WebSocketWrapper::getLastMessage()
{
    return m_lastMessage;
}

QWebSocket *WebSocketWrapper::getSocket() const
{
    return m_pSocket;
}
