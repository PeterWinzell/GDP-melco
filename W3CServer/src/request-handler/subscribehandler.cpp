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
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDateTime>
#include "unsubnotifier.h"
#include "subscribehandler.h"
#include "subscriptions.h"

//QMutex SubscribeHandler::locking;


SubscribeHandler::SubscribeHandler(QObject* parent, QSharedPointer<VSSSignalInterface> signalInterface, VISSRequest* vissrequest, WebSocketWrapper *client):
    RequestHandler(parent, signalInterface, vissrequest,client),m_dosubscription(true)
{
}

void SubscribeHandler::processRequest()
{
    connect(m_pClient->getSocket(), &QWebSocket::disconnected, this, &SubscribeHandler::socketDisconnected);

    //Add to subscriptions and store subscriptions ID
    Subscriptions* subscriptions = Subscriptions::getInstance();
    m_subId = subscriptions -> addSubcription(this);

    qDebug() << " processing subscribe handler requests";

    //Get filter/time

    //Send subscription response
    //Format response on JSON format
    QString successMessage = getSubscriptionSuccessJson();

    //Send message to client
    //locking.lock();
    m_pClient->sendTextMessage(successMessage);
    //locking.unlock();

    while (m_dosubscription)
    {
        //Get latest value of subscribed signal
        QString value = getSignalValue(m_pVissrequest->getSignalPath());

        //Format response on JSON format
        QString message = getSubscriptionNotificationJson(value);

        //Send message to client
        //locking.lock();
        m_pClient->sendTextMessage(message);
        //locking.unlock();
        //Sleep for the period defined by filter
        QThread::currentThread()->sleep(1);
    }

    qDebug() << " subscription cancelled ";
}

void SubscribeHandler::socketDisconnected()
{
    m_dosubscription = false;
}

void SubscribeHandler::unsubscribe()
{
    m_dosubscription = false;
}

WebSocketWrapper* SubscribeHandler::getSocketClient()
{
    return m_pClient;
}

QString SubscribeHandler::getSubscriptionNotificationJson(QString signalValue)
{
    QJsonObject jsonObject;
    jsonObject.insert("action", "subscribing");
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
    jsonObject.insert("requestId", m_pVissrequest->getRequestId());
    jsonObject.insert("subscriptionId", m_subId);
    jsonObject.insert("timestamp", QString::number(QDateTime::currentDateTime().toTime_t() ));

    QJsonDocument jsonDoc(jsonObject);
    return jsonDoc.toJson();
}

QString SubscribeHandler::getSignalValue(QString path)
{
    //Dummy function, just return a random number 0 - 320
    return QString::number(qrand() % 320);
}


