#include "websocketwrapper.h"
#include "logger.h"


//DN DEBUG
#include <QThread>

WebSocketWrapper::WebSocketWrapper(QWebSocket *socket, QMutex* mutex,QObject *parent)
    : QObject(parent), m_pSocket(socket),m_pMutex(mutex)
{
    TRACE("Server", "< WebSocketWrapper > created.");

    //DN DEBUG
    qDebug() << "WebSocketWrapper: task= " << QThread::currentThread();


    connect(socket, &QWebSocket::connected, this, &WebSocketWrapper::socketConnected);
    connect(socket, &QWebSocket::disconnected, this, &WebSocketWrapper::socketDisconnected);

    //DN DEBUG //connect(this, &WebSocketWrapper::sendTextMessage, this, &WebSocketWrapper::sendMessage);

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

        //DN DEBUG
        qDebug() << "WebSocketWrapper::sendTextMessage : m_pSocket = " << m_pSocket << " task= " << QThread::currentThread();

        // flush seems cause problems, why?
        //if (m_pSocket->)
        //{
 //DN DEBUG           m_pSocket->flush(); // well, sometimes (seen in iOS) you really need to flush
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
