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
    static AuthorizationManager* getInstance();

    void insertAuthData(QWebSocket* ,AuthData* obj);
    bool isAuthorized(QWebSocket * thesocket,QString path,QString actions);
    bool deleteAuthData();
    void connectionClosed(QWebSocket* aSocket);

    QList<AuthData*> getAuthData(QWebSocket* thesocket);
signals:

public slots:

private:
    AuthorizationManager(QObject *parent = 0);
    // There can be only one accces at a time
    // static QMutex m_mutex;
    // keep track of authorization data
    static AuthorizationManager * m_instance;
    static QMutex m_mutex;

    QMultiMap<QWebSocket *,QList<AuthData*>> m_authDataTable;

};

#endif // AUTHORIZATIONMANAGER_H
