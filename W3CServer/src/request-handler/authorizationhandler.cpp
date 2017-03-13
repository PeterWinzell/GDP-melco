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
#include "authorizationhandler.h"
#include "jwt-utility/visstokenvalidator.h"
#include <QDebug>


AuthorizationHandler::AuthorizationHandler(QObject* parent, QSharedPointer<VSSSignalInterface> signalInterface, QSharedPointer<VISSRequest> vissrequest,
        WebSocketWrapper *client):
    RequestHandler(parent, signalInterface, vissrequest,client)
{
}

void AuthorizationHandler::processRequest()
{
    qDebug() << " processing authorization handler requests";

    // TODO Make it so that these are added to VISSRequest instead?
    bool userToken = !m_pVissrequest->getTokens().toJsonObject()["authorization"].isNull();
    bool deviceToken = !m_pVissrequest->getTokens().toJsonObject()["www-vehicle-device"].isNull();

    bool authorized;
    if(userToken && !deviceToken)
    {
        authorized = validateUserToken();
    }
    else if(!userToken && deviceToken)
    {
        authorized = validateDeviceToken();
    }
    else if(userToken && deviceToken)
    {
        authorized = validateUserToken() && validateDeviceToken();
    }


    QString time = QString::number(QDateTime::currentDateTime().toTime_t());

    QJsonObject response;
    response.insert("action", "authorize");
    response.insert("requestId", m_pVissrequest->getRequestId());
    response.insert("timestamp", time);

    if (authorized)
    {
        qDebug() << " TOKEN IS VERIFIED \n";

        /* QString zePayload = tokenValidator.getJsonPayload();

        //qDebug() << " token payload is " + (zePayload);

        QJsonDocument doc2;
        doc2 = QJsonDocument::fromJson(zePayload.toUtf8());

        QJsonObject tokenpl = doc2.object();
        QString issuer = tokenpl["iss"].toString();
        //qDebug() << " Token issuer is : " + issuer;

        QString valid_from = tokenpl["ValidFrom"].toString();
        //qDebug() << " ValidFrom : " + valid_from;

        QString valid_to = tokenpl["ValidTo"].toString();
        //qDebug() << " Valid To : " + valid_to;

        QString path = tokenpl["path"].toString();
        //qDebug() << " Signal path is : " + path;

        QString actions = tokenpl["actions"].toString();
        //qDebug() << " Actions are : " + actions;
        */
        // Time to live ?
        //QDateTime::currentDateTime().msecsTo(QDateTime::fromString(valid_to))


        response.insert("TTL", 32);
        // TODO Implement Authorization Success handling
        // TODO Add calculation of time to live.
        // TODO Add to AuthorizationManager
        addAuthorization();
    }
    else
    {
        qDebug() << " TOKEN IS NOT VERIFIED \n";
        response.insert("error", "error");
        // TODO Implement Authorization Failed handling

    }

    QJsonDocument jsonDoc(response);
    QString message = jsonDoc.toJson();

    m_pClient->sendTextMessage(message);
}

bool AuthorizationHandler::validateUserToken()
{
    VissTokenValidator tokenValidator(m_pVissrequest->getTokens().toJsonObject()["authorization"].toString());
    return tokenValidator.validateToken("mydirtysecret");
}
bool AuthorizationHandler::validateDeviceToken()
{
    VissTokenValidator tokenValidator(m_pVissrequest->getTokens().toJsonObject()["www-vehicle-device"].toString());
    return tokenValidator.validateToken("mydirtysecret");
}
void AuthorizationHandler::addAuthorization()
{


}
