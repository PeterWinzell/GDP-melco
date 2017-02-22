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
#ifndef UNSUBSCRIBEALLHANDLER_H
#define UNSUBSCRIBEALLHANDLER_H
#include "requesthandler.h"
#include "vissrequest.h"
#include <QWebSocket>

class UnsubscribeAllHandler : public RequestHandler
{
public:
    explicit UnsubscribeAllHandler(QObject *parent = 0,VISSRequest* vissrequest=0,QWebSocket* client=0);
    void processRequest();
};

#endif // UNSUBSCRIBEALLHANDLER_H
