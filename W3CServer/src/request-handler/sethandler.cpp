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
#include "sethandler.h"
#include "errors/errorresponse.h"

SetHandler::SetHandler(QObject* parent, QSharedPointer<VSSSignalInterface> signalInterface, QSharedPointer<VISSRequest> vissrequest, WebSocketWrapper *client):
    RequestHandler(parent,signalInterface, vissrequest,client)
{
    TRACE("Server", "< SetHandler > created.");
}

void SetHandler::processRequest()
{
    DEBUG("Server","Processing < Set > request.");
    QJsonObject response;
    response.insert("requestId", m_pVissrequest->getRequestId());
    response.insert("action", "set");
    int error = m_pSignalInterface->setSignalValue(m_pVissrequest->getSignalPath(), m_pVissrequest->getValue());

    if(error != 0)
    {
        QJsonObject errorJson;
        ErrorResponse::getInstance()->getErrorJson((ErrorReason)error,&errorJson);
        response.insert("error", errorJson);
    }

    QString time = QString::number(QDateTime::currentDateTime().toTime_t());
    response.insert("timestamp", time);


    QJsonDocument jsonDoc(response);
    m_pClient->sendTextMessage(jsonDoc.toJson());
}
