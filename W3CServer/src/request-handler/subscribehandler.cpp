/**************************************************************************************************************
* Copyright (C) *   2017, peterwinzell
*
*
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.*
*
*
***************************************************************************************************************/
#include <QThread>
#include <QJsonArray>
#include <QDateTime>
#include "subscribehandler.h"

SubscribeHandler::SubscribeHandler(QObject* parent,VISSRequest* vissrequest,QWebSocket *client):
    RequestHandler(parent,vissrequest,client),m_dosubscription(true)
{
}

void SubscribeHandler::processRequest()
{
    connect(p_client, &QWebSocket::disconnected, this, &SubscribeHandler::socketDisconnected);
    qDebug() << " processing get handler requests";

    //Get filter/time

    //Add to subscriptions and store subscriptions ID
    m_subId = 42; //Subscriptions::GetInstance()->add(this);

    //Send subscription response
    //Format response on JSON format
    QString successMessage = getSubscriptionSuccessJson();

    //Send message to client
    p_client->sendTextMessage(successMessage);

    while (m_dosubscription)
    {
        //Get latest value of subscribed signal
        QString value = getVehicleData(p_vissrequest->getSignalPath());

        //Format response on JSON format
        QString message = getSubscriptionNotificationJson(value);

        //Send message to client
        p_client->sendTextMessage(message);

        //Sleep for the period defined by filter
        QThread::currentThread()->sleep(1);
    }

    qDebug() << " subscription cancelled ";
}

void SubscribeHandler::socketDisconnected()
{
{
    m_dosubscription = false;
}

void SubscribeHandler::unsubscribe()
{
    m_dosubscription = false;
}

QString SubscribeHandler::getSubscriptionNotificationJson(QString signalValue)
{
    QJsonObject jsonObject;
    jsonObject.insert("subscriptionId", m_subId);
    jsonObject.insert("value", signalValue);
    jsonObject.insert("timestamp", QString::number(QDateTime::currentDateTime().toTime_t() ));

    QJsonDocument jsonDoc(jsonObject);
    return jsonDoc.toJson();
}

QString SubscribeHandler::getSubscriptionSuccessJson()
{
    QJsonObject jsonObject;
    jsonObject.insert("action", "subscribe");
    jsonObject.insert("requestId", p_vissrequest->getRequestId());
    jsonObject.insert("subscriptionId", m_subId);
    jsonObject.insert("timestamp", QString::number(QDateTime::currentDateTime().toTime_t() ));

    QJsonDocument jsonDoc(jsonObject);
    return jsonDoc.toJson();
}

QString SubscribeHandler::getVehicleData(QString path)
{
    //Dummy function, just return a random number 0 - 320
    return QString::number(qrand() % 320);
}


