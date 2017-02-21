#include "w3ctestclient.h"
#include <QtCore/QDebug>
#include <QtWebSockets/QWebSocket>
#include <QCoreApplication>
#include <QThread>

QT_USE_NAMESPACE

W3cTestClient::W3cTestClient(const QUrl &url, QObject *parent) :
    QObject(parent)
{
    qRegisterMetaType<QAbstractSocket::SocketState>(); //DN: to avoid error "QObject::connect: Cannot queue arguments of type 'QAbstractSocket::SocketState'"

    connect(&m_webSocket, &QWebSocket::connected, this, &W3cTestClient::onConnected);
    typedef void (QWebSocket:: *sslErrorsSignal)(const QList<QSslError> &);
    connect(&m_webSocket, static_cast<sslErrorsSignal>(&QWebSocket::sslErrors),
            this, &W3cTestClient::onSslErrors);
    m_webSocket.open(QUrl(url));
}

void W3cTestClient::onConnected()
{
    qDebug() << "WebSocket connected" << " From thread: " << QThread::currentThreadId();
    connect(&m_webSocket, &QWebSocket::textMessageReceived,
            this, &W3cTestClient::onTextMessageReceived);
    m_webSocket.sendTextMessage(QStringLiteral("Hello, world!"));
}

void W3cTestClient::onTextMessageReceived(QString message)
{
    qDebug() << "Message received:" << message << " From thread: " << QThread::currentThreadId();
    // qApp->quit(); //This quits the main application - should not be done if client runs in separate thread

    // Stop this thread
    QObject::thread()->quit();
}

void W3cTestClient::onSslErrors(const QList<QSslError> &errors)
{
    Q_UNUSED(errors);

     qDebug() << "onSslErrors received" << " From thread: " << QThread::currentThreadId();

    // WARNING: Never ignore SSL errors in production code.
    // The proper way to handle self-signed certificates is to add a custom root
    // to the CA store.

    foreach( const QSslError &error, errors )
    {
        qDebug() << "SSL Error: " << error.errorString();
    }


    m_webSocket.ignoreSslErrors();
}
