#include "gethandler.h"
#include <QDebug>
#include <QJsonDocument>
#include <QJsonArray>
#include "errors/errorresponse.h"
#include "authorization/authorizationmanager.h"

GetHandler::GetHandler(QObject* parent, QSharedPointer<VSSSignalInterface> signalInterface, QSharedPointer<VISSRequest> vissrequest, WebSocketWrapper *client):
    RequestHandler(parent, signalInterface, vissrequest,client)
{
    TRACE("Server", "< GetHandler > created.");
}

GetHandler::~GetHandler()
{
    TRACE("Server", "< GetHandler > destroyed.");
}

int GetHandler::isAuthorized()
{
    AuthorizationManager* aMan = AuthorizationManager::getInstance();
    bool auth = aMan ->isAuthorized(m_pClient ->getSocket(),m_pVissrequest->getSignalPath(),"GET");
    if (auth)
        return 0;
    return 401;
}


void GetHandler::processRequest()
{
    DEBUG("Server", "Processing < Get > request.");
    QJsonObject response;
    response.insert("requestId", m_pVissrequest->getRequestId());
    response.insert("action", "get");

    QJsonArray values;

    int error = isAuthorized(); //TODO: move to parent implementation of processRequest right now pure virtual func.
    if (error == 0) // We are authorized to go ahead and process the request
    {
        error = m_pSignalInterface->getSignalValue(m_pVissrequest->getSignalPath(), values);
    }

    if(error == 0)
    {
        // There are several branches, keep whole array as it is.
        if(values.size() > 1)
        {
            response.insert("value", values);
        }
        // It's one branch...
        else
        {
            QJsonObject obj = values.first().toObject();

            // ... with several leaves, get the single object.
            if(obj.keys().count() > 1)
            {
                response.insert("value", obj);
            }
            // ... with a single leave, get the value and insert it directly to the value.
            else
            {
                QJsonValue val = obj.value(obj.keys().first());

                if(val.isDouble())
                {
                    response.insert("value", val.toDouble());
                }
                else if(val.isBool())
                {
                    response.insert("value", val.toBool());
                }
                else
                {
                    response.insert("value", val.toString());
                }
            }
        }
    }
    else
    {
        DEBUG("Server", "Request contained something bad.");
        // TODO Need to be able to differentiate between different errors.
        QJsonObject errorJson;
        ErrorResponse::getInstance()->getErrorJson((ErrorReason)error,&errorJson);
        response.insert("error", errorJson);
    }

    QString time = QString::number(QDateTime::currentDateTime().toTime_t());
    response.insert("timestamp", time);


    QJsonDocument jsonDoc(response);
    m_pClient->sendTextMessage(jsonDoc.toJson());
}

