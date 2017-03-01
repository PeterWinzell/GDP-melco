#include "w3ctestclient.h"
#include "getvisstestdatajson.h"
#include <QtCore/QDebug>
#include <QtWebSockets/QWebSocket>
#include <QCoreApplication>
#include <QJsonParseError>
#include <QTime>
#include <QJsonObject>

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

   QString subMess = GetVissTestDataJson::getTestDataString(requesttype::SUBSCRIBE);
   m_webSocket.sendTextMessage(subMess);

   qDebug() << " JSON SENT TO SERVER IS: " + subMess;
}

void W3cTestClient::onTextMessageReceived(QString message)
{
    // qDebug() << "Message received:" << message << "\n";
    //parse message
    QJsonParseError parseError;
    QJsonDocument  jsonDocument;
    QJsonObject    jsonObject;

    jsonDocument = QJsonDocument::fromJson(message.toUtf8(),&parseError);
    jsonObject = jsonDocument.object();
    if (parseError.error == QJsonParseError::NoError)
    {
        QString actionString =jsonObject["action"].toString();

        if (actionString == "subscribe")
        {
            QString path = jsonObject["path"].toString();
            m_subscriptionid  = jsonObject["subscriptionId"].toInt();
            qDebug() << path + "succesfully subscribed to";
        }
        else if (actionString == "unsubscribe")
        {

        }
        else if (actionString == "unsubscibeAll")
        {

        }
        else
        {

            int subId = jsonObject["subscriptionId"].toInt();
            QString valueStr  =  jsonObject["value"].toString();
            int value = valueStr.toInt();
            QString timeString  = jsonObject["timestamp"].toString();
            QDateTime time_t = QDateTime::fromTime_t(timeString.toInt());

            if (subId == m_subscriptionid)
            {
                qDebug() << " currentTime is : " + time_t.toString();
                qDebug() << " The value is :   " << value;
            }
        }
    }
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
