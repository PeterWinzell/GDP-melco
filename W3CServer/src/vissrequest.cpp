#include "vissrequest.h"

VISSRequest::VISSRequest()
{
    m_action = ERROR;
}
VISSRequest::VISSRequest(QJsonObject& json)
{
    m_jsonObject = json;

    QString val = m_jsonObject["action"].toString();

    if (val == "get")
    {
        m_action = GET;
    }
    else if (val == "set")
    {
        m_action = SET;
    }
    else if (val == "subscribe")
    {
        m_action = SUBSCRIBE;
    }
    else if (val == "unsubscribe")
    {
        m_action = UNSUBSCRIBE;
    }
    else if (val == "unsubscribeAll")
    {
        m_action = UNSUBSCRIBEALL;
    }
    else if (val == "authorize")
    {
        m_action = AUTHORIZE;
    }
    else if (val == "getVSS")
    {
        m_action = GETVSS;
    }
    else
    {
        m_action = ERROR;
    }

    m_requestId = m_jsonObject["requestId"].toString();
}

QJsonObject VISSRequest::getJsonObject()
{
    return m_jsonObject;
}

void VISSRequest::invalidateRequest()
{
    m_action = ERROR;
}
void VISSRequest::setRequestValues()
{

    switch (m_action)
    {
    case GET:
        m_signalPath = m_jsonObject["path"].toString();
        break;
    case SET:
        m_signalPath = m_jsonObject["path"].toString();
        m_value = m_jsonObject["value"].toVariant();
        break;
    case SUBSCRIBE:
        m_signalPath = m_jsonObject["path"].toString();

        if(!m_jsonObject["filters"].isUndefined())
            m_filters = m_jsonObject["filters"].toVariant();

        break;
    case UNSUBSCRIBE:
        m_subscriptionId = m_jsonObject["subscriptionId"].toString();
        break;
    case UNSUBSCRIBEALL:
        break;
    case AUTHORIZE:
        m_tokens = m_jsonObject["tokens"].toObject();
        break;
    case GETVSS:
        m_signalPath = m_jsonObject["path"].toString();
        break;
    case ERROR:
    default:
        break;
    }
}

vss_request VISSRequest::getAction()
{
    return m_action;
}

QString VISSRequest::getRequestId()
{
    return m_requestId;
}

QString VISSRequest::getSignalPath()
{
    return m_signalPath;
}

QVariant VISSRequest::getValue()
{
    return m_value;
}

QString VISSRequest::getSubscriptionId()
{
    return m_subscriptionId;
}

QVariant VISSRequest::getTokens()
{
    return m_tokens;
}
