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
#ifndef ERRORRESPONSE_H
#define ERRORRESPONSE_H
#include <QJsonObject>
#include <QMultiMap>

/*
304 (Not Modified)	not_modified	No changes have been made by the server.
400 (Bad Request)	bad_request	The server is unable to fulfil the client request because the request is malformed.
401 (Unauthorised)	user_token_expired	User token has expired.
401 (Unauthorised)	user_token_invalid	User token is invalid.
401 (Unauthorised)	user_token_missing	User token is missing.
401 (Unauthorised)	device_token_expired	Device token has expired.
401 (Unauthorised)	device_token_invalid	Device token is invalid.
401 (Unauthorised)	device_token_missing	Device token is missing.
401 (Unauthorised)	too_many_attempts	The client has failed to authenticate too many times.
401 (Unauthorised)	read_only	The desired signal cannot be set since it is a read only signal.
403 (Forbidden)	user_forbidden	The user is not permitted to access the requested resource. Retrying does not help.
403 (Forbidden)	user_unknown	The user is unknown. Retrying does not help.
403 (Forbidden)	device_forbidden	The device is not permitted to access the requested resource. Retrying does not help.
403 (Forbidden)	device_unknown	The device is unknown. Retrying does not help.
404 (Not Found)	invalid_path	The specified data path does not exist.
404 (Not Found)	private_path	The specified data path is private and the request is not authorised to access signals on this path.
404 (Not Found)	invalid_subscriptionId	The specified subscription was not found.
406 (Not Acceptable)	not_acceptable	The server is unable to generate content that is acceptable to the client
429 (Too Many Requests)	too_many_requests	The client has sent the server too many requests in a given amount of time.
502 (Bad Gateway)	bad_gateway	The server was acting as a gateway or proxy and received an invalid response from an upstream server.
503 (Service Unavailable)	service_unavailable	The server is currently unable to handle the request due to a temporary overload or scheduled maintenance (which may be alleviated after some delay).
504 (Gateway Timeout)	gateway_timeout	The server did not receive a timely response from an upstream server it needed to access in order to complete the request.
*/

enum ErrorReason{
    not_modified,
    bad_request,
    user_token_invalid,
    user_token_missing,
    device_token_expired,
    device_token_invalid,
    device_token_missing,
    too_many_attempts,
    read_only,
    user_forbidden,
    user_unknown,
    device_forbidden,
    device_unknown,
    invalid_path,
    private_path,
    invalid_subscriptionId,
    not_acceptable,
    too_many_requests,
    bad_gateway,
    service_unavailable,
    gateway_timeout
};

class ErrorResponse
{
public:
    static ErrorResponse* getInstance();
    void getErrorJson(ErrorReason anError,QJsonObject* errorJson);
private:

    void initErrorTable();
    static ErrorResponse* m_Pinstance;
    static QMutex m_mutex;
    ErrorResponse();
    ~ErrorResponse();

    QMultiMap<ErrorReason,int> m_errortable; // replace int with int + errorMessage
};

#endif // ERRORRESPONSE_H
