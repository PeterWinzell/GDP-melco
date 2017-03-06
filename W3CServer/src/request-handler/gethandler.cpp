#include "gethandler.h"
#include <QDebug>
#include <QJsonDocument>


GetHandler::GetHandler(QObject* parent, QSharedPointer<VSSSignalInterface> signalInterface, VISSRequest* vissrequest, WebSocketWrapper *client):
    RequestHandler(parent, signalInterface, vissrequest,client)
{
    qDebug() << " GetHandler is created.";
}

GetHandler::~GetHandler()
{
    qDebug() << " GetHandler is dying...";
}

void GetHandler::processRequest()
{
    qDebug() << " processing get handler requests";


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

