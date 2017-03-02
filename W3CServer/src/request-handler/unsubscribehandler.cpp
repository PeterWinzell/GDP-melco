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
#include <QJsonObject>
#include <QJsonDocument>
#include "unsubscribehandler.h"
#include "subscriptions.h"


UnsubscribeHandler::UnsubscribeHandler(QObject* parent,VISSRequest* vissrequest,QWebSocket *client):
    RequestHandler(parent,vissrequest,client)
{
}


QString UnsubscribeHandler::Responsebuilder(bool valid)
{
    QJsonObject jsonresponse;
    jsonresponse.insert("action","unsubscribe");
    jsonresponse.insert("subscriptionId",p_vissrequest -> getSubscriptionId());
    jsonresponse.insert("requestId",p_vissrequest -> getRequestId());

    if (!valid)
    {
        QJsonObject errorObject;
        errorObject.insert("number",404);
        errorObject.insert("reason","invalid_subcriptionId");
        errorObject.insert("message","The specified subscription was not found. ");
        jsonresponse.insert("error",errorObject);
    }

    jsonresponse.insert("timestamp",(int)QDateTime::currentDateTime().toTime_t() );

    QJsonDocument jsonDoc(jsonresponse);
    return jsonDoc.toJson();
}

void UnsubscribeHandler::processRequest()
{
    qDebug() << " processing unsubscribe handler requests";

    // retrive on ongoing subscriptions
    Subscriptions* subs = Subscriptions::getInstance();
    bool valid = true;
    if (subs)
    {
        QString subIdStr = p_vissrequest -> getSubscriptionId();
        int subId = subIdStr.toInt();
        valid = subs -> unsubscribe(subId,p_client); // should kill subscription thread
    }

    // send response to client
    p_client -> sendTextMessage( Responsebuilder(valid) );
}
