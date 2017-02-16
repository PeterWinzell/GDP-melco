#include "vissrequest.h"

VISSRequest::VISSRequest()
{
    action = ERROR;
}
VISSRequest::VISSRequest(QJsonObject& json)
{
    jsonObject = json;

    QString val = jsonObject["action"].toString();

    if      (val == "get")           { action = GET; }
    else if (val == "set")           { action = SET; }
    else if (val == "subscribe")     { action = SUBSCRIBE; }
    else if (val == "unsubscribe")   { action = UNSUBSCRIBE; }
    else if (val == "unsubscribeAll"){ action = UNSUBSCRIBEALL; }
    else if (val == "authorize")     { action = AUTHORIZE; }
    else if (val == "getVSS")        { action = GETVSS; }
    else                             { action = ERROR; }

    requestId = jsonObject["requestId"].toString();
}

QJsonObject VISSRequest::getJsonObject(){
    return jsonObject;
}

void VISSRequest::invalidateRequest(){
    action = ERROR;
}
void VISSRequest::setRequestValues(){

    switch (action) {
        case GET:
            signalPath = jsonObject["path"].toString();
            break;
        case SET:
            signalPath = jsonObject["path"].toString();
            value = jsonObject["value"].toVariant();
            break;
        case SUBSCRIBE:
            signalPath = jsonObject["path"].toString();
            if(!jsonObject["filters"].isUndefined())
                filters = jsonObject["filters"].toVariant();
            break;
        case UNSUBSCRIBE:
            subscriptionId = jsonObject["subscriptionId"].toString();
            break;
        case UNSUBSCRIBEALL:
            break;
        case AUTHORIZE:
            tokens = jsonObject["tokens"].toObject();
            break;
        case GETVSS:
            signalPath = jsonObject["path"].toString();
            break;
    }
}


vss_request VISSRequest::getAction(){
    return action;
}

QString VISSRequest::getRequestId(){
    return requestId;
}

QString VISSRequest::getSignalPath(){
    return signalPath;
}

QVariant VISSRequest::getValue(){
    return value;
}

QString VISSRequest::getSubscriptionId(){
    return subscriptionId;
}

QVariant VISSRequest::getTokens(){
    return tokens;
}
