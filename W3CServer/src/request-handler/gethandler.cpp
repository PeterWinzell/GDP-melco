#include "gethandler.h"
#include <QDebug>
#include <QJsonDocument>

GetHandler::GetHandler(QObject* parent, QSharedPointer<VSSSignalInterface> signalInterface, QSharedPointer<VISSRequest> vissrequest, WebSocketWrapper *client):
    RequestHandler(parent, signalInterface, vissrequest,client)
{
    TRACE("Server", "< GetHandler > created.");
}

GetHandler::~GetHandler()
{
    TRACE("Server", "< GetHandler > destroyed.");
}

void GetHandler::processRequest()
{
    DEBUG("Server", "Processing < Get > request.");

    QString key = m_pVissrequest->getSignalPath();
    QString value = m_pSignalInterface->getSignalValue(key);
    QString time = QString::number(QDateTime::currentDateTime().toTime_t());

    QJsonObject response = QJsonObject(m_pVissrequest->getJsonObject());

    response.insert("value", value);
    response.insert("timestamp", time);

    QJsonDocument jsonDoc(response);
    QString message = jsonDoc.toJson();

    m_pClient->sendTextMessage(message);
}

