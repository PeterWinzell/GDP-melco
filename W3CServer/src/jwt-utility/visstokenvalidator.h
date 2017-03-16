#ifndef VISSTOKENVALIDATOR_H
#define VISSTOKENVALIDATOR_H
#include "qjsonwebtoken.h"
#include <QString>

class VissTokenValidator
{
public:
    VissTokenValidator(QString aToken);
    QString getJsonPayload();
    bool validateToken(QString aSecret);
private:
    QString m_token;
    QJsonWebToken m_jsonwebtoken;
};

#endif // VISSTOKENVALIDATOR_H
