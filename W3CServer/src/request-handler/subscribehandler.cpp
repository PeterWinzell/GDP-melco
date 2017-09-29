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
#include <QCoreApplication>
#include <QEventLoop>
#include "unsubnotifier.h"
#include "subscribehandler.h"
#include "subscriptions.h"
#include <QDebug>
#include "errors/errorresponse.h"
const int SubscribeHandler::m_defaultIntervalMs = 100;

SubscribeHandler::SubscribeHandler(QObject* parent, QSharedPointer<VSSSignalInterface> signalInterface, QSharedPointer<VISSRequest> vissrequest,
                                   WebSocketWrapper *client): RequestHandler(parent, signalInterface, vissrequest,client),m_dosubscription(true)
{
    TRACE("Server", "< SubscribeHandler > created.");
    m_pSignalInterface = signalInterface;
}

void SubscribeHandler::processRequest()
{
    DEBUG("Server","Processing < Subscribe > request.");
    connect(m_pClient->getSocket(), &QWebSocket::disconnected, this, &SubscribeHandler::socketDisconnected);

    //Add to subscriptions and store subscriptions ID
    Subscriptions* subscriptions = Subscriptions::getInstance();
    m_subId = subscriptions -> addSubcription(this);

    //Setup filter params
    initializeFilter();

    //Send subscription response
    //Format response on JSON format
    QString successMessage = getSubscriptionSuccessJson();

    //Send message to client
    m_pClient->sendTextMessage(successMessage);

    while (m_dosubscription)
    {
        bool dosend = false;
        QJsonObject response;
        response.insert("action", "subscription");
        response.insert("subscriptionId", m_subId);
        response.insert("timestamp", QString::number(QDateTime::currentDateTime().toTime_t()));

        QJsonArray values(m_lastValues);

        //Get latest value of subscribed signal(s)
        int error = m_pSignalInterface->getSignalValue(m_pVissrequest->getSignalPath(), values);
        if(error == 0)
        {
            if (values != m_lastValues)
            {
                dosend = true;
                m_lastValues = values;

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
                            if (isFilterPass(val.toDouble()))
                            {
                                response.insert("value", val.toDouble());
                            }
                            else
                            {
                                dosend = false;
                            }
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
        }
        else
        {
            DEBUG("Server", "Request contained something bad.");
            // TODO Need to be able to differentiate between different errors.
            QJsonObject errorJson;
            ErrorResponse::getInstance()->getErrorJson((ErrorReason)error,&errorJson);
            response.insert("error", errorJson);
            dosend = true;
        }

        if (dosend)
        {
            //Format response on JSON format and send
            QJsonDocument jsonDoc(response);
            m_pClient->sendTextMessage(jsonDoc.toJson());
        }

        //Let the event loop process events so that signals are not blocked
        QCoreApplication::processEvents(QEventLoop::AllEvents);

        //Sleep for the period defined by filter
        QThread::currentThread()->msleep(m_filter.intervalMs);
    }

    DEBUG("Server","Subscription cancelled.");
}

void SubscribeHandler::socketDisconnected()
{
    DEBUG("Server","Socket disconnected during subscription.");
    m_dosubscription = false;
}

void SubscribeHandler::unsubscribe()
{
    DEBUG("Server","Subscription cancelled due to unsubscription.");
    m_dosubscription = false;
}

WebSocketWrapper* SubscribeHandler::getSocketClient()
{
    return m_pClient;
}

QString SubscribeHandler::getSubscriptionNotificationJson(QString signalValue)
{
    QJsonObject jsonObject;
    jsonObject.insert("action", "subscription");
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

bool SubscribeHandler::isFilterPass(double value)
{
    int diff = abs(value - m_lastValue);
    m_lastValue = value;
    //Check whether value is within range and from last value is big enough
    return ((value >= m_filter.rangeMin) &&
            (value <= m_filter.rangeMax) &&
            (diff >= m_filter.minChange));
}

void SubscribeHandler::initializeFilter()
{
    // qDebug() << "initializeFilter(): Enter";

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
        //qDebug() << "initializeFilter(): filters found";
        TRACE("Server", "Filters found in request.");
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
            m_filter.intervalMs = filterList["interval"].toInt();

            //qDebug() << "initializeFilter(): interval = " << m_filter.intervalMs;
        }

        // Handle range
        if(!filterList["range"].isNull())
        {
            // qDebug() << "initializeFilter(): range found";

            // Range can consist of "above", "below" or both
            QJsonObject range = filterList["range"].toObject();

            if(!range["above"].isNull())
            {
                m_filter.rangeMin = range["above"].toInt();

                //qDebug() << "initializeFilter(): above = " << m_filter.rangeMin;
            }
            if(!range["below"].isNull())
            {
                m_filter.rangeMax = range["below"].toInt();

                //qDebug() << "initializeFilter(): below = " << m_filter.rangeMax;
            }
        }

        // Handle minchange
        if(!filterList["minChange"].isNull())
        {
            m_filter.minChange = filterList["minChange"].toInt();

            //qDebug() << "initializeFilter(): minChange = " << m_filter.minChange;
        }
    }
}

QString SubscribeHandler::getSignalValue(QString path)
{
    //Dummy function, just return a random number 0 - 320
    (void)path;
    return QString::number(qrand() % 320);
}



