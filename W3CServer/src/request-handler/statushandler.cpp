#include "statushandler.h"

StatusHandler::StatusHandler(QObject* parent, QSharedPointer<VSSSignalInterface> signalInterface, QSharedPointer<VISSRequest> vissrequest, WebSocketWrapper *client):
    RequestHandler(parent,signalInterface, vissrequest,client)
{
}

void StatusHandler::processRequest()
{
    qDebug() << "Processing status handler request";


    QJsonObject response = QJsonObject(m_pVissrequest->getJsonObject());
    response.insert("timestamp", time);

    QJsonDocument jsonDoc(response);
    QString message = jsonDoc.toJson();

    m_pClient->sendTextMessage(message);
}
StatusHandler::~StatusHandler()
{
    qDebug() << " StatusHandler is dying...";
}
