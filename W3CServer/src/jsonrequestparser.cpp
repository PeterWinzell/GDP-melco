#include "jsonrequestparser.h"
#include "logger.h"

JSONRequestParser::JSONRequestParser(bool debug, QObject *parent) : QObject(parent)
{
    m_debug = debug;
}
QSharedPointer<VISSRequest> JSONRequestParser::parseJson(QString json)
{
    QSharedPointer<VISSRequest> request = QSharedPointer<VISSRequest>(new VISSRequest(json), &QObject::deleteLater);
    request.data()->setJsonDocument(QJsonDocument::fromJson(json.toUtf8(), &m_parseError));

    if(!isValidJson())
    {
        WARNING("Server", "Json invalid.");
        return request;
    }
    request->setJsonObject(request->getJsonDocument().object());

    if(!validateAction(request.data())) { return request; }

    switch (request->getAction())
    {
        case GET:
            if(!validateGetRequest(request.data())) { request->invalidateRequest(); }
            break;
        case SET:
            if(!validateSetRequest(request.data())) { request->invalidateRequest(); }
            break;
        case SUBSCRIBE:
            if(!validateSubscribeRequest(request.data())) { request->invalidateRequest(); }
            break;
        case UNSUBSCRIBE:
            if(!validateUnsubscribeRequest(request.data())) { request->invalidateRequest(); }
            break;
        case UNSUBSCRIBEALL:
            if(!validateUnsubscribeRequest(request.data())) { request->invalidateRequest(); }
            break;
        case AUTHORIZE:
            if(!validateAuthorizeRequest(request.data())) { request->invalidateRequest(); }
            break;
        case GETMETADATA:
            if(!validateGetMetadataRequest(request.data())) { request->invalidateRequest(); }
            break;
        case STATUS:
            if(!validateStatusRequest(request.data())) { request->invalidateRequest(); }
        break;
        case ERROR: // Just to remove compilation warning.
            request->invalidateRequest();
            break;
    }
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
bool JSONRequestParser::validateGetRequest(VISSRequest* request)
{
    QJsonValue valueId = request->getJsonObject()["requestId"];
    QJsonValue valuePath = request->getJsonObject()["path"];

    bool valid = (validateId(valueId) && validatePath(valuePath));

    if(!valid) { return false; }
    else
    {
        request->setRequestId(valueId.toString());
        request->setSignalPath(valuePath.toString());
        return true;
    }
}

/*
    {
    "action": "set",
    "path": "Signal.Drivetrain.InternalCombustionEngine.RPM",
    "value": 2000,
    "requestId": "8912"
    }
*/
bool JSONRequestParser::validateSetRequest(VISSRequest* request)
{
    QJsonValue valueId = request->getJsonObject()["requestId"];
    QJsonValue valuePath = request->getJsonObject()["path"];
    QJsonValue valueValue = request->getJsonObject()["value"];

    bool valid = (validateId(valueId) && validatePath(valuePath) && validateValue(valueValue));

    if(m_debug) { qDebug() << "validateSetRequest result" << (valid); }

    if(!valid) { return false; }
    else
    {
        request->setRequestId(valueId.toString());
        request->setSignalPath(valuePath.toString());
        request->setValue(QVariant(valueValue));
        return true;
    }
}

/*
    {
    "action": "subscribe",
    "path": "Signal.Drivetrain.Transmission.TripMeter",
    "requestId": "5264"
    }
*/
bool JSONRequestParser::validateSubscribeRequest(VISSRequest* request)
{
    QJsonValue valueId = request->getJsonObject()["requestId"];
    QJsonValue valuePath = request->getJsonObject()["path"];
    QJsonValue valueFilters = request->getJsonObject()["filters"];

    bool valid = (validateId(valueId) && validatePath(valuePath) && validateFilters(valueFilters));

    if(m_debug) { qDebug() << "validateSubscribeRequest result" << (valid); }

    if(!valid) { return false; }
    else
    {
        request->setRequestId(valueId.toString());
        request->setSignalPath(valuePath.toString());
        if(!valueFilters.isNull() && !valueFilters.isUndefined())
        {
            request->setFilters(valueFilters.toVariant());
        }
        return true;
    }
}

/*
    {
    "action": "unsubscribe",
    "subscriptionId": "102",
    "requestId": "5264"
    }
*/
bool JSONRequestParser::validateUnsubscribeRequest(VISSRequest* request)
{
    QJsonValue valueId = request->getJsonObject()["requestId"];
    QJsonValue valueSubscriptionId = request->getJsonObject()["subscriptionId"];

    bool valid = (validateId(valueId) && validateSubscriptionId(valueSubscriptionId));

    if(m_debug) { qDebug() << "validateUnsubscribeRequest result" << (valid); }

    if(!valid) { return false; }
    else
    {
        request->setRequestId(valueId.toString());
        request->setSubscriptionId(valueSubscriptionId.toString());
        return true;
    }
}

/*
    {
        "action": "unsubscribeAll",
        "requestId": "3468"
    }
*/
bool JSONRequestParser::validateUnsubscribeAllRequest(VISSRequest* request)
{
    QJsonValue valueId = request->getJsonObject()["requestId"];

    bool valid = (validateId(valueId));

    if(m_debug) { qDebug() << "validateUnsubscribeAllRequest result" << (valid); }

    if(!valid) { return false; }
    else
    {
        request->setRequestId(valueId.toString());
        return true;
    }
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
bool JSONRequestParser::validateAuthorizeRequest(VISSRequest* request)
{
    QJsonValue valueId = request->getJsonObject()["requestId"];
    QJsonValue valueTokens = request->getJsonObject()["tokens"];

    bool valid = (validateId(valueId) && validateTokens(valueTokens));

    if(m_debug) { qDebug() << "validateAuthorizeRequest result" << (valid); }

    if(!valid) { return false; }
    else
    {
        request->setRequestId(valueId.toString());
        request->setTokens(valueTokens.toObject());
        return true;
    }
}

/*
    {
    "action": "getVSS",
    "path": "Signal.Drivetrain.InternalCombustionEngine.RPM",
    "requestId": "3874"
    }
*/
bool JSONRequestParser::validateGetMetadataRequest(VISSRequest* request)
{
    QJsonValue valueId = request->getJsonObject()["requestId"];
    QJsonValue valuePath = request->getJsonObject()["path"];

    bool valid = (validateId(valueId));

    if(m_debug) { qDebug() << "validateGetMetadataRequest result" << (valid); }

    if(!valid) { return false; }
    else
    {
        request->setRequestId(valueId.toString());
        request->setSignalPath(valuePath.toString());
        return true;
    }
}

bool JSONRequestParser::validateStatusRequest(VISSRequest *request)
{
    QJsonValue valueId = request->getJsonObject()["requestId"];

    bool valid = (validateId(valueId));

    if(m_debug) { qDebug() << "validateStatusRequest result" << (valid); }

    if(!valid) { return false; }
    else
    {
        request->setRequestId(valueId.toString());
        return true;
    }
}


// Validate each of the different parts of a request
bool JSONRequestParser::validateAction(VISSRequest* request)
{
    QJsonValue valueAction = request->getJsonObject()["action"];

    bool valid = valueAction.isString();

    if(m_debug) { qDebug() << "validateAction result" << (valid); }
    if(!valid) { return false; }

    QString val = valueAction.toString();

    if (val == "get")
    {
        request->setAction(GET);
    }
    else if (val == "set")
    {
        request->setAction(SET);
    }
    else if (val == "subscribe")
    {
        request->setAction(SUBSCRIBE);
    }
    else if (val == "unsubscribe")
    {
        request->setAction(UNSUBSCRIBE);
    }
    else if (val == "unsubscribeAll")
    {
        request->setAction(UNSUBSCRIBEALL);
    }
    else if (val == "authorize")
    {
        request->setAction(AUTHORIZE);
    }
    else if (val == "getMetadata")
    {
        request->setAction(GETMETADATA);
    }
    else if (val == "status")
    {
        request->setAction(STATUS);
    }
    else
    {
        request->setAction(ERROR);
    }
    return valid;
}

bool JSONRequestParser::validatePath(QJsonValue value)
{
    QString path = value.toString();
    if(path.isEmpty()) { TRACE("Server", "validatePath path is empty"); return false; }

    QStringList splitPath = path.split('.');

    foreach(QString item, splitPath)
    {
        // Check for empty string. Caused by ex. two dots.
        if(item.isEmpty())
        {
            TRACE("Server", "validatePath string is empty");
            return false;
        }

        // Check if string is single asterisk.
        if(item.length() == 1 && item[0] != '*')
        {
            TRACE("Server", "validatePath string contains several asterisks");
            return false;
        }

        // Check if string contains only text.
        QRegExp regex("^[a-zA-Z]+$");
        if(item.length() > 1 && !regex.exactMatch(item))
        {
            TRACE("Server", "validatePath string is not just simple text");
            return false;
        }
    }

    //(\w+|\*)

    // TODO Add Regex to check path?

    return true;
}

bool JSONRequestParser::validateValue(QJsonValue value)
{
    // TODO Validate the different structures of values? Currently just checks if it's there.
    return !value.isNull() && !value.isUndefined() && value.toVariant() != QVariant().Invalid ;
}

bool JSONRequestParser::validateId(QJsonValue value)
{
    QVariant id = value.toVariant();

    if(id == QVariant().Invalid) { return false; }
    bool parseOk = false;
    id.toInt(&parseOk);

    if(!parseOk)
    {
        QRegExp uuid("^[0-9A-F]{8}-[0-9A-F]{4}-[1-5][0-9A-F]{3}-[89AB][0-9A-F]{3}-[0-9A-F]{12}$");
        return uuid.exactMatch(id.toString());
    }

    return true;
}

bool JSONRequestParser::validateSubscriptionId(QJsonValue value)
{
    return value.isString() || value.isNull();
}

bool JSONRequestParser::validateFilters(QJsonValue value)
{
    if(!value.isNull())
    {
        return value.isObject();
    }
    else
    {
        return true;
    }
}

bool JSONRequestParser::validateTokens(QJsonValue value)
{
    // TODO Validte the diffent tokens.
    return value.isObject();
}

bool JSONRequestParser::isValidJson()
{
    //qDebug() << parseError.errorString();
    return m_parseError.error == QJsonParseError::NoError;
}

JSONRequestParser::~JSONRequestParser()
{
    // Nothing needed atm?
}

