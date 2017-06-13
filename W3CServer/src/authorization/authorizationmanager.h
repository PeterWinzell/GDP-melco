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
#ifndef AUTHORIZATIONMANAGER_H
#define AUTHORIZATIONMANAGER_H

#include <QObject>
#include <QWebSocket>
#include <QList>
#include "authdata.h"

class AuthorizationManager : public QObject
{
    Q_OBJECT
public:
    explicit AuthorizationManager(QObject *parent = 0);
    bool insertAuthData(QWebSocket* , AuthData& obj);
    bool isAuthorized(QWebSocket*,QJsonObject actions);
    bool deleteAuthData(QWebSocket* , QJsonObject& obj);
    QList<QJsonObject>* getAuthData(QWebSocket* thesocket);
signals:

public slots:

private:
    typedef QList<AuthData*> AuthList;
    QMultiMap<QWebSocket *,AuthList> m_authDataTable;

};

#endif // AUTHORIZATIONMANAGER_H
