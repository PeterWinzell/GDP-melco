#include "subscribeunsubscribealltestcase.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QTimer>

SubscribeUnsubscribeAllTestCase::SubscribeUnsubscribeAllTestCase(QString testClientId, int& requestId, QObject *parent)
    : TestCaseBase(testClientId, requestId, parent)
{

}

void SubscribeUnsubscribeAllTestCase::startTest(QWebSocket* webSocket)
{
    m_webSocket = webSocket;

    connect(m_webSocket, &QWebSocket::textMessageReceived, this, &SubscribeUnsubscribeAllTestCase::onTextMessageReceived);

    QString subMess1 = GetVissTestDataJson::getTestDataString(requesttype::SUBSCRIBE, QString::number(m_requestId));
    QString subMess2 = GetVissTestDataJson::getTestDataString(requesttype::SUBSCRIBE, QString::number(m_requestId++));
    QString subMess3 = GetVissTestDataJson::getTestDataString(requesttype::SUBSCRIBE, QString::number(m_requestId++));

    m_webSocket->sendTextMessage(subMess1);
    m_webSocket->sendTextMessage(subMess2);
    m_webSocket->sendTextMessage(subMess3);

    QTimer::singleShot(10000,this,SLOT(unsubscribeAll()));
}
void SubscribeUnsubscribeAllTestCase::forceStop()
{

}

void SubscribeUnsubscribeAllTestCase::onTextMessageReceived(QString message)
{
    DEBUG(m_testClientId,"Message received");
    TRACE(m_testClientId,message);

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
            /*if(m_currentTest != TestCase::SUBSCRIBE_UNSUBSCRIBE && m_currentTest != TestCase::SUBSCRIBEALL_UNSUBSCRIBEALL)
            {
                WARNING(m_testClientId,"Received Subcribe action when not requested.");
                passTestRun(false);
                return;
            }
*/
            QString requestId = jsonObject["requestId"].toString();
            QJsonObject errorObject = jsonObject["error"].toObject();
            if (!errorObject.empty())
            {
                WARNING(m_testClientId,"Failed to subscribe.");
                QString errorMessage = errorObject["message"].toString();
                DEBUG(m_testClientId,QString("Error! Request ID : %1, Message : %2").arg(requestId, errorMessage));

                emit finished(false);
                return;
            }
            QString path = jsonObject["path"].toString();

            //cache last received subscribeId
            m_unsubscribeCachedSubscriptionId = jsonObject["subscriptionId"].toString();

            INFO(m_testClientId, QString("Successfully subscribed to signal : %1, sub ID : %2").arg(path, m_unsubscribeCachedSubscriptionId));
        }
        else if (actionString == "subscribing")
        {
            /*if(m_currentTest != TestCase::SUBSCRIBE_UNSUBSCRIBE && m_currentTest != TestCase::SUBSCRIBEALL_UNSUBSCRIBEALL)
            {
                WARNING(m_testClientId,"Received Subscribing action when not requested.");
                emit finished(false);
                return;
            }
*/
            QString requestId = jsonObject["requestId"].toString();
            QJsonObject errorObject = jsonObject["error"].toObject();
            if (!errorObject.empty())
            {
                WARNING(m_testClientId,"Subscribing failed.");
                QString errorMessage = errorObject["message"].toString();
                DEBUG(m_testClientId,QString("Error! Request ID : %1, Message : %2").arg(requestId, errorMessage));

                emit finished(false);
                return;
            }
            //QString subId = jsonObject["subscriptionId"].toString();
            //QString valueStr  =  jsonObject["value"].toString();
            //int value = valueStr.toInt();
            //QString timeString  = jsonObject["timestamp"].toString();
            //QDateTime time_t = QDateTime::fromTime_t(timeString.toInt());

            //qDebug() << " subscriptionId is : " + subId << " \n";
            //qDebug() << " currentTime is : " + time_t.toString() << " \n";
            //qDebug() << " The value is :   " << value << " \n";

            TRACE(m_testClientId,"Received Subscribing message.");

            //passTestRun(true); //Add a counter of some sort before we pass?
        }
        else if (actionString == "unsubscribeAll")
        {
            /*if(m_currentTest != TestCase::SUBSCRIBEALL_UNSUBSCRIBEALL)
            {
                WARNING(m_testClientId,"Received UnsubcribeAll action when not requested.");
                emit finished(false);
                return;
            }*/

            QString requestId = jsonObject["requestId"].toString();
            QJsonObject errorObject = jsonObject["error"].toObject();
            if (!errorObject.empty())
            {
                WARNING(m_testClientId,"Unsubscribe all failed.");
                QString errorMessage = errorObject["message"].toString();
                DEBUG(m_testClientId,QString("Error! Request ID : %1, Message : %2").arg(requestId, errorMessage));

                emit finished(false);
                return;
            }
            INFO(m_testClientId,"Successfully unsubscribed from all signals.");

            emit finished(true);
        }
        else{
            WARNING(m_testClientId, "Received incorrect action.");
            emit finished(false);
            return;
        }
    }
    else
    {
        emit  finished(false);
    }
}

void SubscribeUnsubscribeAllTestCase::pendingTestTimeout()
{
    TRACE(m_testClientId,"Pending test ready, continue with next test.");

    //m_pendingTest = false;
    //runTest();
}

void SubscribeUnsubscribeAllTestCase::unsubscribeAll()
{
    INFO(m_testClientId,"Unsubscribing All");
    //m_pendingTest = true;
    QString usubscribeAllMess = GetVissTestDataJson::getTestDataString(requesttype::UNSUBSCRIBEALL, QString::number(m_requestId++));
    m_webSocket->sendTextMessage(usubscribeAllMess);

    // wait a while for subscriptions to end
    //QTimer::singleShot(2000,this,SLOT(pendingTestTimeout()));
}
