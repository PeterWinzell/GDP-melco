#include "w3ctestclient.h"
#include "getvisstestdatajson.h"
#include <QtCore/QDebug>
#include <QtWebSockets/QWebSocket>
#include <QCoreApplication>
#include <QThread>

QT_USE_NAMESPACE

W3cTestClient::W3cTestClient(const QUrl &url, QObject *parent) :
    QObject(parent)
{
    connect(&m_webSocket, &QWebSocket::connected, this, &W3cTestClient::onConnected);
    typedef void (QWebSocket:: *sslErrorsSignal)(const QList<QSslError> &);
    connect(&m_webSocket, static_cast<sslErrorsSignal>(&QWebSocket::sslErrors),
            this, &W3cTestClient::onSslErrors);
    m_webSocket.open(QUrl(url));
}

void W3cTestClient::onConnected()
{
    qDebug() << "WebSocket connected";
    connect(&m_webSocket, &QWebSocket::textMessageReceived,
            this, &W3cTestClient::onTextMessageReceived);

    //QString authMess = GetVissTestDataJson::getTestDataString(requesttype::AUTHORIZE);
    //qDebug() << " JSON SENT TO SEVER IS: " + authMess;
    // m_webSocket.sendTextMessage(authMess);

    QString subMess = GetVissTestDataJson::getTestDataString(requesttype::GET);

    m_webSocket.sendTextMessage(subMess);

    qDebug() << " JSON SENT TO SERVER IS: " + subMess;
}

void W3cTestClient::onTextMessageReceived(QString message)
{
    qDebug() << "Message received:" << message;

    QThread::msleep(10);

    m_webSocket.sendTextMessage(GetVissTestDataJson::getTestDataString(requesttype::GET));

    //qApp->quit();
}

void W3cTestClient::onSslErrors(const QList<QSslError> &errors)
{
    Q_UNUSED(errors);

    // WARNING: Never ignore SSL errors in production code.
    // The proper way to handle self-signed certificates is to add a custom root
    // to the CA store.

    foreach( const QSslError &error, errors )
    {
        qDebug() << "SSL Error: " << error.errorString();
    }


    m_webSocket.ignoreSslErrors();
}
