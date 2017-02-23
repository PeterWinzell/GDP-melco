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
#include <QThread>
#include "subscribehandler.h"



SubscribeHandler::SubscribeHandler(QObject* parent,VISSRequest* vissrequest,QWebSocket *client):
    RequestHandler(parent,vissrequest,client),m_dosubscription(true){
}

void SubscribeHandler::processRequest(){
    connect(p_client, &QWebSocket::disconnected, this, &SubscribeHandler::socketDisconnected);
    qDebug() << " processing get handler requests";

    while (m_dosubscription){
        p_client->sendTextMessage(" you are subscribing ");
        QThread::currentThread()->sleep(1);
    }
    qDebug() << " subscription cancelled ";
}

void SubscribeHandler::socketDisconnected(){
    m_dosubscription = false;
}
