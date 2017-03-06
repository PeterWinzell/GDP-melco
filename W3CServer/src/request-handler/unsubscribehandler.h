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
#ifndef UNSUBSCRIBEHANDLER_H
#define UNSUBSCRIBEHANDLER_H
#include "requesthandler.h"
#include "vissrequest.h"
#include <QWebSocket>

class UnsubscribeHandler : public RequestHandler
{
public:
    explicit UnsubscribeHandler(QObject *parent, QSharedPointer<VSSSignalInterface> signalInterface, QSharedPointer<VISSRequest> vissrequest,WebSocketWrapper* client=0);
    void processRequest();
    QString Responsebuilder(bool valid);
};

#endif // UNSUBSCRIBEHANDLER_H
