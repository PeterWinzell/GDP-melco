#include "w3ctestclient.h"
#include <QtCore/QDebug>
#include <QtWebSockets/QWebSocket>
#include <QCoreApplication>

QT_USE_NAMESPACE

W3cTestClient::W3cTestClient(const QUrl &url, QObject *parent) :
    QObject(parent)
{
    // add connection setup code here...
}

void W3cTestClient::onConnected()
{
    // add on connected code here...
}

void W3cTestClient::onTextMessageReceived(QString message)
{
    // add onTextMessageRecieved code here
}

void W3cTestClient::onSslErrors(const QList<QSslError> &errors)
{
   // add onSSlErrors code here
}
