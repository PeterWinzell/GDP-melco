#include <QThreadPool>
#include "statushandler.h"
#include "subscriptions.h"
#include "w3cserver.h"

StatusHandler::StatusHandler(QObject* parent, QSharedPointer<VSSSignalInterface> signalInterface, QSharedPointer<VISSRequest> vissrequest, WebSocketWrapper *client):
    RequestHandler(parent,signalInterface, vissrequest,client)
{
    TRACE("Server", "< StatusHandler > created.");
}

StatusHandler::~StatusHandler()
{
    TRACE("Server", "< StatusHandler > destroyed.");
}

void StatusHandler::processRequest()
{
    DEBUG("Server", "Processing < Status > request.");

    QJsonObject response = QJsonObject(m_pVissrequest->getJsonObject());
    response.insert("clients", W3CServer::m_nrOfClients);
    response.insert("threads", QThreadPool::globalInstance()->activeThreadCount());
    response.insert("subcriptions", Subscriptions::getInstance()->getSubscriptionCount());
    QString time = QString::number(QDateTime::currentDateTime().toTime_t());
    response.insert("timestamp", time);

    QJsonDocument jsonDoc(response);
    QString message = jsonDoc.toJson();

    m_pClient->sendTextMessage(message);
}
