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
#include "errorresponse.h"

ErrorResponse* ErrorResponse::m_Pinstance = nullptr;
QMutex ErrorResponse::m_mutex;

ErrorResponse* ErrorResponse::getInstance()
{
    QMutexLocker mutexlock(&m_mutex);
    if (m_Pinstance == nullptr)
    {
        m_Pinstance = new ErrorResponse();
     }
    return m_Pinstance;
}


ErrorResponse::ErrorResponse()
{
    initErrorTable();
}

ErrorResponse::~ErrorResponse()
{
    //TODO: clean the errortable...
}

void ErrorResponse::initErrorTable()
{
    m_errortable.insert(ErrorReason::bad_request,400);
    m_errortable.insert(ErrorReason::user_token_invalid,401);
}

void ErrorResponse::getErrorJson(ErrorReason anError,QJsonObject* errorObject)
{
    errorObject->insert("number",m_errortable.value(anError));
    errorObject->insert("reason",anError);
    errorObject->insert("message",""); //TODO: add message strings later later
}
