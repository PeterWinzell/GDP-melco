#include "gethandler.h"
#include <QDebug>
#include <QJsonDocument>


GetHandler::GetHandler(QObject* parent, VISSRequest* vissrequest, WebSocketWrapper *client):
    RequestHandler(parent,vissrequest,client)
{
}

void GetHandler::processRequest()
{
    qDebug() << " processing get handler requests";


    QString key = m_pVissrequest->getSignalPath();

    // TODO: get value from Signal VSSS implementation

    QString value = "1234";

    QString time = QString::number(QDateTime::currentDateTime().toTime_t());

    QJsonObject response = QJsonObject(m_pVissrequest->getJsonObject());

    response.insert("value", value);
    response.insert("timestamp", time);

    QJsonDocument jsonDoc(response);
    QString message = jsonDoc.toJson();

    m_pClient->sendTextMessage(message);

}
