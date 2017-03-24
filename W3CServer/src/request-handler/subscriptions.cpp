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
#include "subscriptions.h"
#include "unsubnotifier.h"

//Initialize static members
Subscriptions* Subscriptions::m_instance = nullptr;
int Subscriptions::m_subscriptionIdCounter = 0;
QMutex Subscriptions::m_mutex;

Subscriptions* Subscriptions::getInstance()
{
    QMutexLocker mutexlock(&m_mutex);
    if (m_instance == nullptr)
    {
        m_subscriptionIdCounter = 0;
        m_instance = new Subscriptions();
     }
    return m_instance;
}

QString Subscriptions::addSubcription(SubscribeHandler* handler)
{
    QMutexLocker lock(&m_mutex);

    m_subscriptionIdCounter++;

    QString subidStr = QString::number(m_subscriptionIdCounter);
    QWebSocket* client = handler -> getSocketClient() -> getSocket();
    UnsubNotifier* usubNotifier = new UnsubNotifier();
    // connect unsubscription
    connect(usubNotifier,&UnsubNotifier::unsubscribe,handler,&SubscribeHandler::unsubscribe);
    // handle unsubscribe
    m_notifiers.insert(subidStr,usubNotifier);
    //handle unsubscribe all
    m_clientsubscriptions.insert(client,subidStr);

    return QString::number(m_subscriptionIdCounter);
}

bool Subscriptions::unsubscribe(QString subscriptionId)
{
    QMutexLocker lock(&m_mutex);



    if (m_notifiers.contains(subscriptionId))
    {
        UnsubNotifier* notifier = m_notifiers.find(subscriptionId).value();

        notifier -> unsubScribe();
    }
    else
    {
        return false;
    }

    m_notifiers.remove(subscriptionId);

    for (auto it = m_clientsubscriptions.begin(); it != m_clientsubscriptions.end();)
    {
        if (it.value() == subscriptionId) {
            it = m_clientsubscriptions.erase(it);
        } else {
            ++it;
        }
    }

    return true;
}

bool Subscriptions::unsubscribeAll(WebSocketWrapper* client)
{
    QMutexLocker lock(&m_mutex);

    QWebSocket* client_ws = client -> getSocket();

    if (client_ws == nullptr)
        return false;

    QList<QString> ids = m_clientsubscriptions.values(client_ws);

    if (ids.size() == 0)
    {
        return false;
    }

    for (int i = 0; i < ids.size(); ++i)
    {
        UnsubNotifier* notifier = m_notifiers.find(ids.value(i)).value();
        notifier -> unsubScribe();
    }

    for(int i = 0; i < ids.size(); ++i)
    {
        m_notifiers.remove(ids.value(i));
    }

   int removeditems = m_clientsubscriptions.remove(client_ws);
   if (removeditems <= 0)
       qDebug() << " error in removing subcriptions ";

   return true;
}
