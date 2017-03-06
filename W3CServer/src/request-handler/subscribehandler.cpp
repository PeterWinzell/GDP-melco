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

const int SubscribeHandler::m_defaultIntervalMs = 1000;


SubscribeHandler::SubscribeHandler(QObject* parent, QSharedPointer<VSSSignalInterface> signalInterface, QSharedPointer<VISSRequest> vissrequest, WebSocketWrapper *client):
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

    //Setup filter params
    initializeFilter();

    //Send subscription response
    //Format response on JSON format
    QString successMessage = getSubscriptionSuccessJson();

    //Send message to client
    m_pClient->sendTextMessage(successMessage);

    while (m_dosubscription)
    {
        //Get latest value of subscribed signal
        QString value = getSignalValue(m_pVissrequest->getSignalPath());

        if (isFilterPass(value))
        {
            m_lastValue = value.toInt();

            //Format response on JSON format
            QString message = getSubscriptionNotificationJson(value);

            qDebug() << "Sending message:" << message;

            //Send message to client
            m_pClient->sendTextMessage(message);
        }

        //Sleep for the period defined by filter
        QThread::currentThread()->msleep(m_filter.intervalMs);
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

bool SubscribeHandler::isFilterPass(QString valueString)
{
    int value = valueString.toInt();
    int diff = abs(value - m_lastValue);

    //Check whether value is within range and from last value is big enough
    return ((value >= m_filter.rangeMin) &&
            (value <= m_filter.rangeMax) &&
            (diff >= m_filter.minChange));
}

void SubscribeHandler::initializeFilter()
{
    qDebug() << "initializeFilter(): Enter";

    m_filter.intervalMs = m_defaultIntervalMs;
    m_filter.rangeMin = 0;
    m_filter.rangeMax = std::numeric_limits<int>::max();
    m_filter.minChange = 0;
    m_lastValue = 0;

    //Parse filter from request
    QJsonObject jsonObject = m_pVissrequest->getJsonObject();

    //First, check that filter is included in the request
    if(!jsonObject["filters"].isNull())
    {
        qDebug() << "initializeFilter(): filters found";

        //Filter can be one or more tags, where supported tags are:
        //
        // "interval" , for example:
        //          "filters": { "interval": 100 }
        //
        // "range" , for example:
        //          "filters": { "range": { "above": 100, "below": 200 } }
        //          "filters": { "range": { "below": 100 } }
        //
        // "minChange" , for example:
        //          "filters": { "minChange": 100 }
        //          "filters": { "range": { "below": 200 }, "minChange": 20}

        QJsonObject filterList = jsonObject["filters"].toObject();

        // Handle interval
        if(!filterList["interval"].isNull())
        {
            m_filter.intervalMs = filterList["interval"].toString().toInt();

            qDebug() << "initializeFilter(): interval = " << m_filter.intervalMs;
        }

        // Handle range
        if(!filterList["range"].isNull())
        {
            qDebug() << "initializeFilter(): range found";

            // Range can consist of "above", "below" or both
            QJsonObject range = filterList["range"].toObject();

            if(!range["above"].isNull())
            {
                m_filter.rangeMin = range["above"].toString().toInt();

                qDebug() << "initializeFilter(): above = " << m_filter.rangeMin;
            }
            if(!range["below"].isNull())
            {
                m_filter.rangeMax = range["below"].toString().toInt();

                qDebug() << "initializeFilter(): below = " << m_filter.rangeMax;
            }
        }

        // Handle minchange
        if(!filterList["minChange"].isNull())
        {
            m_filter.minChange = filterList["minChange"].toString().toInt();

            qDebug() << "initializeFilter(): minChange = " << m_filter.minChange;
        }
    }
}

QString SubscribeHandler::getSignalValue(QString path)
{
    //Dummy function, just return a random number 0 - 320
    (void)path;
    return QString::number(qrand() % 320);
}



