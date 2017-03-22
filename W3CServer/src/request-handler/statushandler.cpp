#include <QThreadPool>
#include "statushandler.h"
#include "subscriptions.h"
#include "w3cserver.h"

StatusHandler::StatusHandler(QObject* parent, QSharedPointer<VSSSignalInterface> signalInterface, QSharedPointer<VISSRequest> vissrequest, WebSocketWrapper *client):
    RequestHandler(parent,signalInterface, vissrequest,client)
{
}

void StatusHandler::processRequest()
{
    qDebug() << "Processing status handler request";


    QJsonObject response = QJsonObject(m_pVissrequest->getJsonObject());
    response.insert("clients", W3CServer::m_nrOfClients);
    response.insert("threads", QThreadPool::globalInstance()->activeThreadCount());
    response.insert("subcriptions", Subscriptions::getInstance()->getSubscriptionCount());

    response.insert("timestamp", time);

    QJsonDocument jsonDoc(response);
    QString message = jsonDoc.toJson();

    m_pClient->sendTextMessage(message);
}
StatusHandler::~StatusHandler()
{
    qDebug() << " StatusHandler is dying...";
}
