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
#include "errors/errorresponse.h"
#include "authorization/authdata.h"
#include "authorization/authorizationmanager.h"
#include <QDebug>

AuthorizationHandler::AuthorizationHandler(QObject* parent, QSharedPointer<VSSSignalInterface> signalInterface, QSharedPointer<VISSRequest> vissrequest,
        WebSocketWrapper *client): RequestHandler(parent, signalInterface, vissrequest,client)
{
    TRACE("Server", "< AuthorizeHandler > created.");
}

QString AuthorizationHandler::AddToAuthManager(QString zePayload)
{
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

    // calulate TTL time to live in milliseconds
    qint64 validFrom = valid_from.toLongLong();
    qint64 validTo = valid_to.toLongLong();
    qint64 timeToLive = validTo - validFrom; // yes use current time but wtf
    TRACE("Server",QString("Auth TTL is : %1").arg(timeToLive));

    QString time = QString::number(QDateTime::currentDateTime().toTime_t());


    QJsonObject response;
    response.insert("action", "authorize");
    response.insert("requestId", m_pVissrequest -> getRequestId());
    response.insert("TTL", timeToLive);
    response.insert("timestamp", time);

    QJsonDocument jsonDoc(response);
    QString message = jsonDoc.toJson();

    // create authData object and add to managerclass.
    AuthData authdata(path,actions,validFrom,validTo);
    AuthorizationManager* authMan = AuthorizationManager::getInstance();
    authMan -> insertAuthData(m_pClient -> getSocket(),&authdata);

    return message;
}

QString AuthorizationHandler::getAuthErrorMessage()
{
    QJsonObject errorJson;
    ErrorResponse::getInstance()->getErrorJson(ErrorReason::user_token_invalid,&errorJson);

   QJsonObject responseJson;

   responseJson.insert("error",errorJson);
   responseJson.insert("action","authorize");
   responseJson.insert("requestId",m_pVissrequest -> getRequestId());

   QJsonDocument jsonDoc(responseJson);
   QString message = jsonDoc.toJson();

   return message;
}

QString AuthorizationHandler::ValidateToken()
{
    TRACE("Server", QString("Token : %1").arg(m_pVissrequest->getTokens().toJsonObject()["authorization"].toString()));

    VissTokenValidator tokenValidator(m_pVissrequest->getTokens().toJsonObject()["authorization"].toString());

    QString responseM;
    if (tokenValidator.validateToken("mydirtysecret"))
    {
        TRACE("Server", "Token verified.");
        responseM = AddToAuthManager(tokenValidator.getJsonPayload());
    }
    else
    {
        TRACE("Server", "Token not verified.");
        responseM = getAuthErrorMessage();
    }

    return responseM;
}

void AuthorizationHandler::processRequest()
{
    DEBUG("Server", "Processing < Authorize > request.");

    QString message = ValidateToken();
    m_pClient->sendTextMessage(message);
}
