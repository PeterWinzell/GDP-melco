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

SetHandler::SetHandler(QObject* parent, QSharedPointer<VSSSignalInterface> signalInterface, QSharedPointer<VISSRequest> vissrequest, WebSocketWrapper *client):
    RequestHandler(parent,signalInterface, vissrequest,client)
{
    TRACE("Server", "< SetHandler > created.");
}

void SetHandler::processRequest()
{
    DEBUG("Server","Processing < Set > request.");

    QString key = m_pVissrequest->getSignalPath();
    QVariant value = m_pVissrequest->getValue();

    TRACE("Server", QString("Key : %1, Value : %2").arg(key, value.toString()));

    m_pSignalInterface->setSignalValue(key, value);

    QString time = QString::number(QDateTime::currentDateTime().toTime_t());

    QJsonObject response = QJsonObject(m_pVissrequest->getJsonObject());

    response.remove("path");
    response.remove("value");
    response.insert("timestamp", time);

    QJsonDocument jsonDoc(response);
    QString message = jsonDoc.toJson();

    m_pClient->sendTextMessage(message);
}
