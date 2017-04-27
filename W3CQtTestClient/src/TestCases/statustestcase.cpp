#include "statustestcase.h"
#include <QJsonDocument>
#include <QJsonObject>

StatusTestCase::StatusTestCase(QString testClientId, int& requestId, QObject *parent)
    : TestCaseBase(testClientId, requestId, parent)
{

}

void StatusTestCase::startTest(QWebSocket* webSocket)
{
    m_webSocket = webSocket;
    connect(m_webSocket, &QWebSocket::textMessageReceived, this, &StatusTestCase::onTextMessageReceived);

    QString subMess = GetVissTestDataJson::getTestDataString(requesttype::STATUS, QString::number(m_requestId++));

    m_webSocket->sendTextMessage(subMess);
}
void StatusTestCase::forceStop()
{

}

void StatusTestCase::onTextMessageReceived(QString message)
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

        if (actionString != "status")
        {
            WARNING(m_testClientId,"Received incorrect action.");
            emit finished(false);
            return;
        }

        QString requestId = jsonObject["requestId"].toString();
        QJsonObject errorObject = jsonObject["error"].toObject();
        if (!errorObject.empty())
        {
            WARNING(m_testClientId,"Status failed.");
            QString errorMessage = errorObject["message"].toString();
            DEBUG(m_testClientId,QString("Error! Request ID : %1, Message : %2").arg(requestId, errorMessage));
            emit finished(false);
            return;
        }

        QString receivedValue = jsonObject["value"].toString();

        INFO(m_testClientId,"Successfully got status.");
        emit  finished(true);
    }
    else
    {
        emit  finished(false);
    }
}
