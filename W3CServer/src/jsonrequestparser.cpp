#include "jsonrequestparser.h"

JSONRequestParser::JSONRequestParser(QString json, bool debug, QObject *parent) : QObject(parent)
{
    m_debug = debug;
    jsonDocument = QJsonDocument::fromJson(json.toUtf8(), &parseError);

    parseJson();
}
void JSONRequestParser::parseJson(){
    if(!isValidJson()){
        if(m_debug) qDebug() << "json invalid";
        request = new VISSRequest();
        return;
    }

    jsonObject = jsonDocument.object();

    if(!validateAction()){
        if(m_debug) qDebug() << "action invalid";
        request = new VISSRequest();
        return;
    }
    if(!validateId()){
        if(m_debug) qDebug() << "requestId invalid";
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
    if(m_debug) qDebug() << "validateGetRequest result" << (validatePath());
    return validatePath();
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
    if(m_debug) qDebug() << "validateSetRequest result" << (validatePath() && validateValue());
    return validatePath() && validateValue();
}

/*
    {
    "action": "subscribe",
    "path": "Signal.Drivetrain.Transmission.TripMeter",
    "requestId": "5264"
    }
*/
bool JSONRequestParser::validateSubscribeRequest(){
    if(m_debug) qDebug() << "validateSubscribeRequest result" << (validatePath());
    return validatePath();
}

/*
    {
    "action": "unsubscribe",
    "subscriptionId": "102",
    "requestId": "5264"
    }
*/
bool JSONRequestParser::validateUnsubscribeRequest(){
    if(m_debug) qDebug() << "validateUnsubscribeRequest result" << (validateSubscriptionId());
    return validateSubscriptionId();
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
    if(m_debug) qDebug() << "validateAuthorizeRequest result" << (validateTokens());
    return validateTokens();
}

/*
    {
    "action": "getVSS",
    "path": "Signal.Drivetrain.InternalCombustionEngine.RPM",
    "requestId": "3874"
    }
*/
bool JSONRequestParser::validateGetVSSRequest(){
    if(m_debug) qDebug() << "validateAuthorizeRequest result" << (validatePath());
    return validatePath();
}


// Validate each of the different parts of a request
bool JSONRequestParser::validateAction(){
    if(m_debug) qDebug() << jsonObject["action"];
    return jsonObject["action"].isString();
}

bool JSONRequestParser::validatePath(){
    QString path = jsonObject["path"].toString();
    if(path.isEmpty()) return false;

    QStringList splitPath = path.split('.');

    foreach(QString item, splitPath){
           // Check for empty string. Caused by ex. two dots.
           if(item.isEmpty()) {
               if(m_debug) qDebug() << "validatePath string is empty";
               return false;
           }

           // Check if string is single asterisk.
           if(item.length() == 1 && item[0] != '*') {
               if(m_debug) qDebug() << "validatePath string contains several asterisks";
               return false;
           }

           // Check if string contains only text.
           QRegExp regex("^[a-zA-Z]+$");
           if(!regex.exactMatch(item)) {
               qDebug() << item;
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
    return !jsonObject["value"].isUndefined() && jsonObject["value"].toVariant() != QVariant().Invalid ;
}

bool JSONRequestParser::validateId(){
    QVariant id = jsonObject["requestId"].toVariant();

    if(id == QVariant().Invalid) return false;
    bool parseOk = false;
    id.toInt(&parseOk);

    if(!parseOk){
        QRegExp uuid("^[0-9A-F]{8}-[0-9A-F]{4}-[1-5][0-9A-F]{3}-[89AB][0-9A-F]{3}-[0-9A-F]{12}$");
        return uuid.exactMatch(id.toString());
    }

    return true;
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

