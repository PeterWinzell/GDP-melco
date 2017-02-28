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
#include "subscriptions.h"
#include "unsubnotifier.h"

int Subscriptions::addSubcription(SubscribeHandler* handler)
{
    QMutexLocker lock(&m_mutex);

    m_subscriptionIdCounter++;
    QWebSocket* client = handler -> getSocketClient();
    UnsubNotifier* usubNotifier = new UnsubNotifier();
    // connect unsubscription
    connect(usubNotifier,&UnsubNotifier::unsubscribe,handler,&SubscribeHandler::unsubscribe);
    // handle unsubscribe
    m_notifiers.insert(m_subscriptionIdCounter,usubNotifier);
    //handle unsubscribe all
    m_clientsubscriptions.insert(client,m_subscriptionIdCounter);

    return m_subscriptionIdCounter;
}

bool Subscriptions::unsubscribe(int subscriptionId,QWebSocket* client)
{
    QMutexLocker lock(&m_mutex);


    UnsubNotifier* notifier = m_notifiers.find(subscriptionId).value();
    if (notifier)
    {
        notifier -> unsubScribe();
    }
    else
    {
        return false;
    }

    m_notifiers.remove(subscriptionId);
    m_clientsubscriptions.remove(client);

    return true;
}

bool Subscriptions::unsubscribeAll(QWebSocket* client)
{
    QMutexLocker lock(&m_mutex);

    QList<int> ids = m_clientsubscriptions.values(client);

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

    m_clientsubscriptions.remove(client);

    return true;
}
