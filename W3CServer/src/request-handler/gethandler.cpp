#include "gethandler.h"
#include <QDebug>
#include <QJsonDocument>
#include <QJsonArray>

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

    QString path = m_pVissrequest->getSignalPath();

    QJsonArray value = m_pSignalInterface->getSignalValue(path);
    QString time = QString::number(QDateTime::currentDateTime().toTime_t());

    QJsonObject response = QJsonObject(m_pVissrequest->getJsonObject());

    response.remove("path");

    if(value.count() > 1)
    {
        response.insert("value", value);
    }
    else
    {
        QJsonObject obj = value.first().toObject();
        if(obj.keys().count() > 1)
        {
            response.insert("value", obj);
        }
        else
        {
             QJsonValue val = obj.value(obj.keys().first());

             if(val.isDouble())
                response.insert("value", val.toDouble());
             else if(val.isBool())
                 response.insert("value", val.toBool());
             else
                 response.insert("value", val.toString());
        }
    }

    response.insert("timestamp", time);

    QJsonDocument jsonDoc(response);
    QString message = jsonDoc.toJson();

    m_pClient->sendTextMessage(message);
}

