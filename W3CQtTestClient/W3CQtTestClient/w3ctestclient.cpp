#include "w3ctestclient.h"
#include "getvisstestdatajson.h"
#include <QtCore/QDebug>
#include <QtWebSockets/QWebSocket>
#include <QCoreApplication>
#include <QJsonParseError>
#include <QTime>
#include <QJsonObject>
#include <QTimer>

QT_USE_NAMESPACE

W3cTestClient::W3cTestClient(const QUrl &url, QObject *parent) :
    QObject(parent)
{
    m_test = TestCase::SUBSCRIBE_UNSUBSCRIBE; //default testcase

    connect(&m_webSocket, &QWebSocket::connected, this, &W3cTestClient::onConnected);
    typedef void (QWebSocket:: *sslErrorsSignal)(const QList<QSslError> &);
    connect(&m_webSocket, static_cast<sslErrorsSignal>(&QWebSocket::sslErrors),
            this, &W3cTestClient::onSslErrors);
    m_webSocket.open(QUrl(url));
}

void W3cTestClient::setTest(TestCase test)
{
    m_test = test;
}

void W3cTestClient::onConnected()
{
    qDebug() << "WebSocket connected";
    connect(&m_webSocket, &QWebSocket::textMessageReceived,
            this, &W3cTestClient::onTextMessageReceived);

    //QString authMess = GetVissTestDataJson::getTestDataString(requesttype::AUTHORIZE);
    //qDebug() << " JSON SENT TO SEVER IS: " + authMess;
    // m_webSocket.sendTextMessage(authMess);

    // QString subMess = GetVissTestDataJson::getTestDataString(requesttype::SUBSCRIBE);
    //  m_webSocket.sendTextMessage(subMess);

    switch (m_test)
    {
        case TestCase::SUBSCRIBE_UNSUBSCRIBE:
            RunSubscribeUnsubscribeTest();
            break;

        case TestCase::SUBSCRIBEALL_UNSUBSCRIBEALL:
            RunSubscribeUnsubscribeAllTest();
            break;

        case TestCase::GET_VSS:
            RunGetVssTest();
            break;

        case TestCase::SET_GET:
            break;

        case TestCase::AUTHORIZE_SUCCESS:
            break;

        default:
            break;
    }

    //RunSubscribeUnsubscribeTest();
    //RunSubscribeUnsubscribeAllTest();

}

void W3cTestClient::onTextMessageReceived(QString message)
{
    qDebug() << "Message received:" << message << "\n";
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

            //cache last received subscribeId
            m_unsubscribeCachedSubscriptionId = jsonObject["subscriptionId"].toString();

            QString requestid = jsonObject["requestId"].toString();
            qDebug() << path + "succesfully subscribed to \n ";
            qDebug() << " subscriptionId is : " << m_unsubscribeCachedSubscriptionId << " \n";
            qDebug() << " requestId is : " + requestid << " \n";
        }
        else if (actionString == "unsubscribe")
        {
            QString  subscriptionId = jsonObject["subscriptionId"].toString();
            QJsonObject errorObject = jsonObject["error"].toObject();
            if (!errorObject.empty())
            {
                QString errorMessage = errorObject["message"].toString();
                qDebug() << errorMessage + " subId: " << subscriptionId;
            }
            else
            {
                qDebug() << "succesfully unsuscribed to : " << subscriptionId;
            }
        }
        else if (actionString == "unsubscribeAll")
        {
            QString requestId = jsonObject["requestId"].toString();
            QJsonObject errorObject = jsonObject["error"].toObject();
            if (!errorObject.empty())
            {
                QString errorMessage = errorObject["message"].toString();
                qDebug() << errorMessage + " request was :" << requestId;
            }
            else
            {
                qDebug() << " succesfully performed unsubscribe all ";
            }

        }
        else if (actionString == "subscribing")
        {

            QString subId = jsonObject["subscriptionId"].toString();
            QString valueStr  =  jsonObject["value"].toString();
            int value = valueStr.toInt();
            QString timeString  = jsonObject["timestamp"].toString();
            QDateTime time_t = QDateTime::fromTime_t(timeString.toInt());

            qDebug() << " subscriptionId is : " + subId << " \n";
            qDebug() << " currentTime is : " + time_t.toString() << " \n";
            qDebug() << " The value is :   " << value << " \n";

        }
        else if (actionString == "getVSS")
        {
            QString requestId = jsonObject["requestId"].toString();
            QJsonObject errorObject = jsonObject["error"].toObject();
            if (!errorObject.empty())
            {
                QString errorMessage = errorObject["message"].toString();
                qDebug() << errorMessage + " request was :" << requestId;
            }
            else
            {
                QJsonObject vssObject = jsonObject["vss"].toObject();
                qDebug() << " VSS data received: " << vssObject;
            }
        }
        else
        {


        }
    }
}


/**
 * @brief W3cTestClient::RunSubscribeUnsubscribeTest
 * Testing subscribe and unsubscribe and unsubscribeall scenario
 */
void W3cTestClient::RunSubscribeUnsubscribeTest()
{
    qDebug() << " running usubscribe test \n";

    QString subMess = GetVissTestDataJson::getTestDataString(requesttype::SUBSCRIBE);
    m_webSocket.sendTextMessage(subMess);
    QTimer::singleShot(10000,this,SLOT(unsubscribe()));

}

void W3cTestClient::RunSubscribeUnsubscribeAllTest()
{
    QString subMess1 = GetVissTestDataJson::getTestDataString(requesttype::SUBSCRIBE);
    QString subMess2 = GetVissTestDataJson::getTestDataString(requesttype::SUBSCRIBE);
    QString subMess3 = GetVissTestDataJson::getTestDataString(requesttype::SUBSCRIBE);

    //do 3 subscriptions
    m_webSocket.sendTextMessage(subMess1);
    m_webSocket.sendTextMessage(subMess2);
    m_webSocket.sendTextMessage(subMess3);

    QTimer::singleShot(10000,this,SLOT(unsubscribeAll()));

}

void W3cTestClient::RunGetVssTest()
{
    qDebug() << " running getvss test \n";

    QString subMess = GetVissTestDataJson::getTestDataString(requesttype::GETVSS);
    m_webSocket.sendTextMessage(subMess);
}

void W3cTestClient::unsubscribe()
{
    qDebug() << " sending usubscribe to server \n";
    QString unsubMess = GetVissTestDataJson::getTestDataString(requesttype::UNSUBSCRIBE,m_unsubscribeCachedSubscriptionId);
    m_webSocket.sendTextMessage(unsubMess);
}

void W3cTestClient::unsubscribeAll()
{
    QString usubscribeAllMess = GetVissTestDataJson::getTestDataString(requesttype::UNSUBSCRIBEALL);
    m_webSocket.sendTextMessage(usubscribeAllMess);
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
