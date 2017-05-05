#include "websocketwrapper.h"
#include "logger.h"
#include <QThread>
#include <QCoreApplication>
#include <QEventLoop>


WebSocketWrapper::WebSocketWrapper(QWebSocket *socket, QMutex* mutex,QObject *parent)
    : QObject(parent), m_pSocket(socket),m_pMutex(mutex)
{
    TRACE("Server", "< WebSocketWrapper > created.");

    connect(socket, &QWebSocket::connected, this, &WebSocketWrapper::socketConnected);
    connect(socket, &QWebSocket::disconnected, this, &WebSocketWrapper::socketDisconnected);

    connect(this, &WebSocketWrapper::sendTextMessageSignal, this, &WebSocketWrapper::sendTextMessageSlot, Qt::QueuedConnection);

    // check if socket is connected at first start
    m_connected = socket->isValid();
}

WebSocketWrapper::~WebSocketWrapper()
{
    //QThread::currentThread()->sleep(2);
    TRACE("Server", "< WebSocketWrapper > destroyed.");
}

qint64 WebSocketWrapper::sendTextMessage(const QString message)
{
    qDebug() << "Server WebSocketWrapper::sendTextMessage" << QThread::currentThreadId();
    QMutexLocker locker(m_pMutex); // locking per client
    qint64 bytesSent = 0;

    if (m_connected)
    {
      emit sendTextMessageSignal(message);
    }
    else
    {
        WARNING("Server", "Socket not connected. Unable to send.");
        TRACE("Server", "Message : " + message);
    }

    return bytesSent;
}

void WebSocketWrapper::sendTextMessageSlot(const QString message)
{
    //TRACE("Server WebSocketWrapper::sendTextMessageSlot", QThread::currentThreadId());
    qDebug() << "Server WebSocketWrapper::sendTextMessageSlot" << QThread::currentThreadId();
    QMutexLocker locker(m_pMutex);
    if (m_connected)
    {
        TRACE("sendTextMessageSlotServer", "Sending message : " + message);
       m_pSocket->sendTextMessage(message);
       m_pSocket->flush();
    }
    else
    {
        WARNING("sendTextMessageSlot", "Socket not connected. Unable to send.");
        TRACE("sendTextMessageSlot", "Message : " + message);
    }

    m_lastMessage = message;
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
