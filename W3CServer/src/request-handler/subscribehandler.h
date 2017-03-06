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
#ifndef SUBSCRIBEHANDLER_H
#define SUBSCRIBEHANDLER_H
#include "requesthandler.h"
#include "vissrequest.h"
#include <QWebSocket>
#include <QMutex>

class SubscribeHandler : public RequestHandler
{
    Q_OBJECT
public:
    explicit SubscribeHandler(QObject *parent, QSharedPointer<VSSSignalInterface> signalInterface, QSharedPointer<VISSRequest> vissrequest, WebSocketWrapper* client=0);

    void processRequest();
     WebSocketWrapper* getSocketClient();

public slots:
    void socketDisconnected();
    void unsubscribe();
protected:
    bool m_dosubscription;

private:
    QString m_subId;
    int m_lastValue;
    static const int m_defaultIntervalMs;

    class Filter
    {
    public:
        int rangeMin;
        int rangeMax;
        int intervalMs;
        int minChange;
    };
    Filter m_filter;

    QString getSubscriptionNotificationJson(QString signalValue);
    QString getSubscriptionSuccessJson();
    bool isFilterPass(QString valueString);
    void initializeFilter();
    QString getSignalValue(QString path);
    static QMutex locking;
};

#endif // SUBSCRIBEHANDLER_H
