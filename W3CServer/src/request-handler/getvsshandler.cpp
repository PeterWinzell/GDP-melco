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
#include "getvsshandler.h"

GetVSSHandler::GetVSSHandler(QObject* parent, QSharedPointer<VSSSignalInterface> signalInterface, QSharedPointer<VISSRequest> vissrequest, WebSocketWrapper *client):
    RequestHandler(parent, signalInterface, vissrequest, client)
{
    TRACE("Server", "< GetVSSHandler > created.");
}

void GetVSSHandler::processRequest()
{
    DEBUG("Server", "Processing < GetVSS > request.");

    QString path = m_pVissrequest->getSignalPath();
    QString id = m_pVissrequest->getRequestId();
    QString time = QString::number(QDateTime::currentDateTime().toTime_t());

    QJsonObject vss = m_pSignalInterface->getVSSNode(path);

    QJsonObject response;
    response.insert("vss", vss);
    response.insert("timestamp", time);
    response.insert("action", "getVSS");
    response.insert("id", id);

    QJsonDocument jsonDoc(response);
    QString message = jsonDoc.toJson();

    m_pClient->sendTextMessage(message);
}

