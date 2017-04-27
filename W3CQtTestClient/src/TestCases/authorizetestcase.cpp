#include "authorizetestcase.h"
#include <QJsonDocument>
#include <QJsonObject>

AuthorizeTestCase::AuthorizeTestCase(QString testClientId, int& requestId, QObject *parent)
    : TestCaseBase(testClientId, requestId, parent)
{

}

void AuthorizeTestCase::startTest(QWebSocket* webSocket)
{
    m_webSocket = webSocket;
    connect(m_webSocket, &QWebSocket::textMessageReceived, this, &AuthorizeTestCase::onTextMessageReceived);

    QString subMess = GetVissTestDataJson::getTestDataString(requesttype::AUTHORIZE, QString::number(m_requestId++));

    m_webSocket->sendTextMessage(subMess);
}
void AuthorizeTestCase::forceStop()
{

}

void AuthorizeTestCase::onTextMessageReceived(QString message)
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

        if (actionString != "authorize")
        {
            WARNING(m_testClientId,"Received incorrect action.");
            emit finished(false);
            return;
        }

        QString requestId = jsonObject["requestId"].toString();
        QJsonObject errorObject = jsonObject["error"].toObject();
        if (!errorObject.empty())
        {
            WARNING(m_testClientId,"Authorize failed.");
            QString errorMessage = errorObject["message"].toString();
            DEBUG(m_testClientId,QString("Error! Request ID : %1, Message : %2").arg(requestId, errorMessage));

            emit  finished(false);
            return;
        }

        INFO(m_testClientId,"Successfully authorized.");
        emit  finished(true);
    }
    else
    {
        emit  finished(false);
    }
}
