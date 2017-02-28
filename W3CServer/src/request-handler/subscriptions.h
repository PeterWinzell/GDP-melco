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
#ifndef SUBSCRIPTIONS_H
#define SUBSCRIPTIONS_H
#include <QMutex>
#include <QMutexLocker>
#include <QHash>
#include "subscribehandler.h"
#include "unsubnotifier.h"




class Subscriptions : public QObject
{
    Q_OBJECT

public:

    static Subscriptions* getInstance();

    int addSubcription(SubscribeHandler* handler);
    bool unsubscribe(int subscriptionId,QWebSocket* client);
    bool unsubscribeAll(QWebSocket* client);

signals:
    void unsubscribe();

private:

    //There can be only one accces at a time
   // static QMutex m_mutex;
    //Keep track of subscriptions
    static Subscriptions * m_instance;
    static int m_subscriptionIdCounter;
    static QMutex m_mutex;

    QMap<int,UnsubNotifier*> m_notifiers;
    QMultiMap<QWebSocket*,int>  m_clientsubscriptions;

    Subscriptions(QObject *parent=0):QObject(parent)
    {

    }

};



#endif // SUBSCRIPTIONS_H
