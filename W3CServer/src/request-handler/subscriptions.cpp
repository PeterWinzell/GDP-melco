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

int Subscriptions::addSubcription(SubscribeHandler* handler)
{
    QMutexLocker lock(m_mutex);

    m_subscriptionIdCounter++;
    m_subcriptions.insert(m_subscriptionIdCounter,handler);
    return m_subscriptionIdCounter;
}

void Subscriptions::unsubscribe(int subscriptionId)
{
    m_mutex.lock();
    SubscribeHandler* handler = m_subcriptions.find(subscriptionId);
    if (handler)
    {
        handler -> unsubscribe();
    }

    m_subcriptions.remove(subscriptionId);
    m_mutex.unlock();
}

void Subscriptions::unsubscribeAll(){
    m_mutex.lock();
    //traverse all subscriptions and stop subscribing and remove all
    m_subcriptions.clear();
    m_mutex.unlock();
}
