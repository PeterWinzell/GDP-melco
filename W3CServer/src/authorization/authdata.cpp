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
#include "authdata.h"

AuthData::AuthData(QString path,QString actions,quint64 validFrom,quint64 validTo):
m_path(path),m_actions(actions),m_validFrom(validFrom),m_validTo(validTo)
{
}


bool AuthData::isAuthApproved(QString path,QString action)
{
    return actionMember(action) && (isExactMatch(m_path,path) || isBranchMatch(m_path,path));
}


QString AuthData::findMaxMatch(QString path1, QString path2)
{
    QString matchS="";
    int i = 0;
    while ( (i < path1.length() && i < path2.length()) && (path1.at(i) == path2.at(i)) )
    {
        matchS.append(path1.at(i));
        i++;
    }
    return matchS;
}

bool AuthData::isExactMatch(QString authtree,QString authbranch)
{
    return (authtree == authbranch);
}

bool AuthData::isBranchMatch(QString authtree, QString authbranch)
{
    QString matchS = findMaxMatch(authtree,authbranch);
    int auth_three_length = authtree.length();
    int matchs_length = matchS.length();

    QChar lastchar = authtree.at(auth_three_length - 1 );

    return (lastchar == '*' && matchs_length == (auth_three_length - 1));

}

bool AuthData::actionMember(QString action)
{
    return m_actions.contains(action);
}
