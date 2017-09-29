#include "getvsstestcase.h"
#include <QJsonDocument>
#include <QJsonObject>

GetVSSTestCase::GetVSSTestCase(QString testClientId, int& requestId, QObject *parent)
    : TestCaseBase(testClientId, requestId, parent)
{

}

void GetVSSTestCase::startTest(QWebSocket* webSocket)
{
    m_webSocket = webSocket;
    connect(m_webSocket, &QWebSocket::textMessageReceived, this, &GetVSSTestCase::onTextMessageReceived);

    QString subMess = GetVissTestDataJson::getTestDataString(requesttype::GETVSS, QString::number(m_requestId++));

    m_webSocket->sendTextMessage(subMess);
}
void GetVSSTestCase::forceStop()
{

}

void GetVSSTestCase::onTextMessageReceived(QString message)
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

        if (actionString != "getMetadata")
        {
            WARNING(m_testClientId,"Received incorrect action.");
            emit finished(false);
            return;
        }
        QString requestId = jsonObject["requestId"].toString();
        QJsonObject errorObject = jsonObject["error"].toObject();
        if (!errorObject.empty())
        {
            WARNING(m_testClientId,"Failed to get VSS Data.");
            QString errorMessage = errorObject["message"].toString();
            DEBUG(m_testClientId,QString("Error! Request ID : %1, Message : %2").arg(requestId, errorMessage));

            emit finished(false);
            return;
        }

        //QJsonObject vssObject = jsonObject["vss"].toObject();
        INFO(m_testClientId,"Successfully received VSS Data.");
        TRACE(m_testClientId, jsonObject["vss"].toString());
        emit finished(true);
    }
    else
    {
        emit  finished(false);
    }
}
