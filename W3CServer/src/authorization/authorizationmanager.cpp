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
#include "authorizationmanager.h"

//Initialize static members
QMutex AuthorizationManager::m_mutex;
AuthorizationManager* AuthorizationManager::m_instance = nullptr;

//Singleton pattern
AuthorizationManager* AuthorizationManager::getInstance()
{
    QMutexLocker mutexlock(&m_mutex);
    if (m_instance == nullptr)
    {
        m_instance = new AuthorizationManager();
     }
    return m_instance;
}

AuthorizationManager::AuthorizationManager(QObject *parent) : QObject(parent)
{

}

QList<AuthData*> AuthorizationManager::getAuthData(QWebSocket *thesocket)
{
    QList<AuthData*> list = m_authDataTable.value(thesocket);
    QList<AuthData*> list2;
    if (list.isEmpty())
    {
        m_authDataTable.insert(thesocket,list2);
    }

    return list2;
}

void AuthorizationManager::insertAuthData(QWebSocket * socket, AuthData* obj)
{
    QMutexLocker mutexlock(&m_mutex);
    QList<AuthData*> list = getAuthData(socket);
    list.append(obj);
}

bool AuthorizationManager::isAuthorized(QWebSocket * thesocket,QString path,QString actions)
{
    QMutexLocker mutexlock(&m_mutex);
    QList<AuthData*> list = getAuthData(thesocket);

    bool isAuth = false;
    if (list.isEmpty())
        return isAuth;
    int i = 0;
    while ( (isAuth == false) && (i < list.size()))
    {
        AuthData* data = list.at(i++);
        isAuth = data -> isAuthApproved(path,actions);
    }

    return isAuth;
}

bool AuthorizationManager::deleteAuthData()
{
    //garbage_collect expired auth data

}

void AuthorizationManager::connectionClosed(QWebSocket* aSocket)
{
    QMutexLocker mutexlock(&m_mutex);

    QList<AuthData*> list = getAuthData(aSocket);
    if (!list.isEmpty())
    {
        qDeleteAll(list);
    }
    m_authDataTable.remove(aSocket);
}
