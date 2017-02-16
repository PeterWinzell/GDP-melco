#include "jsonrequestparser.h"

JSONRequestParser::JSONRequestParser(QString json, QObject *parent) : QObject(parent)
{
    jsonDocument = QJsonDocument::fromJson(json.toUtf8(), &parseError);

    parseJson();
}
void JSONRequestParser::parseJson(){


    qDebug() << "parsing";
    if(!isValidJson()){
        qDebug() << "json invalid";
        request = new VISSRequest();
        return;
    }

    jsonObject = jsonDocument.object();

    if(!validateAction()){
        qDebug() << "action invalid";
        request = new VISSRequest();
        return;
    }
    if(!validateId()){
        qDebug() << "requestId invalid";
        request = new VISSRequest();
        return;
    }

    request = new VISSRequest(jsonObject);

    switch (request->getAction()) {
        case GET:
            if(!validateGetRequest()) request->invalidateRequest();
            break;
        case SET:
            if(!validateSetRequest()) request->invalidateRequest();
            break;
        case SUBSCRIBE:
            if(!validateSubscribeRequest()) request->invalidateRequest();
            break;
        case UNSUBSCRIBE:
            if(!validateUnsubscribeRequest()) request->invalidateRequest();
            break;
        case UNSUBSCRIBEALL:
            if(!validateUnsubscribeRequest()) request->invalidateRequest();
            break;
        case AUTHORIZE:
            if(!validateAuthorizeRequest()) request->invalidateRequest();
            break;
        case GETVSS:
            if(!validateGetVSSRequest()) request->invalidateRequest();
            break;
    }

    request->setRequestValues();
}

VISSRequest* JSONRequestParser::getRequest(){
    return request;
}


// Validate the different Requests

/*
    {
    "action": "get",
    "path": "Signal.Drivetrain.InternalCombustionEngine.RPM",
    "requestId": "8756"
    }
*/
bool JSONRequestParser::validateGetRequest(){
    qDebug() << "validateGetRequest result" << (validatePath());
    return validateId() && validatePath();
}

/*
    {
    "action": "set",
    "path": "Signal.Drivetrain.InternalCombustionEngine.RPM",
    "value": 2000,
    "requestId": "8912"
    }
*/
bool JSONRequestParser::validateSetRequest(){
    qDebug() << "validateSetRequest result" << (validatePath() && validateValue());
    return validateId() && validatePath() && validateValue();
}

/*
    {
    "action": "subscribe",
    "path": "Signal.Drivetrain.Transmission.TripMeter",
    "requestId": "5264"
    }
*/
bool JSONRequestParser::validateSubscribeRequest(){
    qDebug() << "validateSubscribeRequest result" << (validatePath());
    return validateId() && validatePath();
}

/*
    {
    "action": "unsubscribe",
    "subscriptionId": "102",
    "requestId": "5264"
    }
*/
bool JSONRequestParser::validateUnsubscribeRequest(){
    qDebug() << "validateUnsubscribeRequest result" << (validatePath() && validateSubscriptionId());
    return validateId() && validatePath() && validateSubscriptionId();
}
/*
    {
        "action": "unsubscribeAll",
        "requestId": "3468"
    }
*/
bool JSONRequestParser::validateUnsubscribeAllRequest(){
    return true; // We have already verified action and id if we have gotten to here.
}
/*
    {
    "action": "authorize",
    "tokens":
    {
        "authorization": "a-token-value"
    },
    "requestId": "1"
    }
*/
bool JSONRequestParser::validateAuthorizeRequest(){
    qDebug() << "validateAuthorizeRequest result" << (validatePath() && validateTokens());
    return validateId() && validatePath() && validateTokens();
}

/*
    {
    "action": "getVSS",
    "path": "Signal.Drivetrain.InternalCombustionEngine.RPM",
    "requestId": "3874"
    }
*/
bool JSONRequestParser::validateGetVSSRequest(){
    qDebug() << "validateAuthorizeRequest result" << (validatePath());
    return validateId() && validatePath();
}


// Validate each of the different parts of a request
bool JSONRequestParser::validateAction(){
    qDebug() << jsonObject["action"];
    return jsonObject["action"].isString();
}

bool JSONRequestParser::validatePath(){
    QString path = jsonObject["path"].toString();
    //QVariant v = path.split('.');
    foreach(QString item, path.split('.')){
           // Check for empty string. Caused by ex. two dots.
           if(item.isEmpty()) {
               qDebug() << "validatePath string is empty";
               return false;
           }

           // Check if string is single asterisk.
           if(item.length() == 1 && item[0] != '*') {
               qDebug() << "validatePath string contains several asterisks";
               return false;
           }

           // Check if string contains only text.
           QRegExp regex("^(\\w+)$");
           if(!regex.captureCount()) {
               qDebug() << "validatePath string is not just simple text";
               return false;
           }

    }

    //(\w+|\*)

    // TODO Add Regex to check path?

    return true;
}

bool JSONRequestParser::validateValue(){
    // TODO Validate the different structures of values? Currently just checks if it's there.
    return !jsonObject["value"].isUndefined();
}

bool JSONRequestParser::validateId(){
    return jsonObject["requestId"].isString();
}

bool JSONRequestParser::validateSubscriptionId(){
    return jsonObject["validateSubscriptionId"].isString() || jsonObject["validateSubscriptionId"].isNull();
}
bool JSONRequestParser::validateFilters(){
    if(!jsonObject["filters"].isUndefined())
        return jsonObject["filters"].isObject();
    else return true;
}
bool JSONRequestParser::validateTokens(){
    // TODO Validte the diffent tokens.
    return jsonObject["tokens"].isObject();
}

bool JSONRequestParser::isValidJson() {
    //qDebug() << parseError.errorString();
    return parseError.error == QJsonParseError::NoError;
}

JSONRequestParser::~JSONRequestParser(){
    // Nothing needed atm?
}

