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
        WebSocketWrapper *client): RequestHandler(parent, signalInterface, vissrequest,client)
{
    TRACE("Server", "< AuthorizeHandler > created.");
}

void AuthorizationHandler::processRequest()
{
    DEBUG("Server", "Processing < Authorize > request.");
    TRACE("Server", QString("Token : %1").arg(m_pVissrequest->getTokens().toJsonObject()["authorization"].toString()));

    VissTokenValidator tokenValidator(m_pVissrequest->getTokens().toJsonObject()["authorization"].toString());

    if (tokenValidator.validateToken("mydirtysecret"))
    {
        TRACE("Server", "Token not verified.");
    }
    else
    {
        TRACE("Server", "Token verified.");
    }

    QString zePayload = tokenValidator.getJsonPayload();

    //TRACE("Server", QString("Token payload : %1").arg(zePayload));

    QJsonDocument doc2;
    doc2 = QJsonDocument::fromJson(zePayload.toUtf8());

    QJsonObject tokenpl = doc2.object();
    QString issuer = tokenpl["iss"].toString();
    TRACE("Server", QString("Token issuer : %1").arg(issuer));

    QString valid_from = tokenpl["ValidFrom"].toString();
    QString valid_to = tokenpl["ValidTo"].toString();
    TRACE("Server", QString("Token valid from : %1, to : %2").arg(valid_from, valid_to));

    QString path = tokenpl["path"].toString();
    TRACE("Server", QString("Token signal path : %1").arg(path));

    QString actions = tokenpl["actions"].toString();
    TRACE("Server", QString("Token actions : %1").arg(actions));




    QString time = QString::number(QDateTime::currentDateTime().toTime_t());

    QJsonObject response;
    response.insert("action", "authorize");
    response.insert("requestId", "3");
    response.insert("TTL", 42);
    response.insert("timestamp", time);

    QJsonDocument jsonDoc(response);
    QString message = jsonDoc.toJson();

    m_pClient->sendTextMessage(message);
}
