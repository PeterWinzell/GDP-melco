#include "visstokenvalidator.h"

VissTokenValidator::VissTokenValidator(QString aToken):
    m_token(aToken)
{

}

QString VissTokenValidator::getJsonPayload()
{
    return m_jsonwebtoken.getPayloadQStr();
}

bool VissTokenValidator::validateToken(QString aSecret)
{
    m_jsonwebtoken = QJsonWebToken::fromTokenAndSecret(m_token,aSecret);
    return m_jsonwebtoken.isValid();
}
