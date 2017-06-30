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
#ifndef AUTHDATA_H
#define AUTHDATA_H

#include <QObject>

/**
 * @brief The AuthData class
 * Used to match client requests against current authorization data.
 * Actions that are authorize are GET,SET,SUBSCRIBE
 */
class AuthData : public QObject
{
    Q_OBJECT
public:
    AuthData(QString path,QString actions,quint64 validFrom,quint64 validTo);

    bool isAuthApproved(QString path,QString actions);
    static AuthData*  getDefaultAuthObject();
private:
    //returns the maximum sequence pattern match of path1 and path2
    QString findMaxMatch(QString path1,QString path2);

    // is the auththree === authbranch.
    bool isExactMatch(QString authtree, QString authbranch);
    // is it a branch match ? authbranch is a genuine subset of authtree
    bool isBranchMatch(QString authtree,QString authbranch);
    // is the action authorized
    bool actionMember(QString action);

    // checks if currenttime is within [validFrom,validTo]
    bool isValid();

    QString m_path;
    QString m_actions;
    quint64 m_validFrom;
    quint64 m_validTo;
};

#endif // AUTHDATA_H
