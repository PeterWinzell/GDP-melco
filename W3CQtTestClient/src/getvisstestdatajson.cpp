#include "getvisstestdatajson.h"
#include "qjsonwebtoken.h"
#include <QDateTime>
#include <QJsonArray>

//int GetVissTestDataJson::m_requestId = 0;
QVariant GetVissTestDataJson::m_setValue = "0";

GetVissTestDataJson::GetVissTestDataJson()
{

}


QString GetVissTestDataJson::getTestDataString(requesttype type, QString requestId, QString subId)
{

    QString testJSON;
    switch(type)
    {
        case requesttype::GET:
            testJSON = getGetJson(requestId);
            break;
        case requesttype::SET:
            testJSON = getSetJson(requestId);
            break;
        case requesttype::SUBSCRIBE:
            testJSON = getSubscriptionJson(requestId);
            break;
        case requesttype::UNSUBSCRIBE:
            testJSON = getUnsubscribe(requestId, subId);
            break;
        case requesttype::UNSUBSCRIBEALL:
            testJSON = getUnsubscribeAll(requestId);
            break;
        case requesttype::GETVSS:
            testJSON = getGetVssJson(requestId);
            break;
        case requesttype::AUTHORIZE:
            testJSON = getAuthJson(requestId);
            break;
        case requesttype::STATUS:
            testJSON = getStatusJson(requestId);
            break;
        case requesttype::GET_MANY:
            testJSON = getGetManyJson(requestId);
            break;
        case requesttype::SET_MANY:
            testJSON = getSetManyJson(requestId);
            break;
    }

    return testJSON;
}

QString GetVissTestDataJson::getSubscriptionJson(QString& requestId)
{
    QJsonObject jsonObject;
    QJsonObject jsonFilterObject;
    QJsonObject jsonRangeObject;

    //client receives data when the value is between 100 and 200 (inclusive)
    //  "filters": { "range": { "above": 100, "below": 200 } },

    jsonObject.insert("action","subscribe");
    jsonObject.insert("path","Signal.Drivetrain.Transmission.Speed");

    //jsonRangeObject.insert("above",50);
    //jsonRangeObject.insert("below",200);
    //jsonFilterObject.insert("range", jsonRangeObject);
    //jsonFilterObject.insert("minChange", 40);
    jsonFilterObject.insert("interval",500);
    jsonObject.insert("filters", jsonFilterObject);

    jsonObject.insert("requestId", requestId);
    jsonObject.insert("timestamp", QString::number(QDateTime::currentDateTime().toTime_t() ));

    QJsonDocument jsonDoc(jsonObject);
    return jsonDoc.toJson();
}

QString GetVissTestDataJson::getGetJson(QString& requestId)
{
    QJsonObject jsonObject;
    jsonObject.insert("action","get");
    jsonObject.insert("path","Signal.ADAS.CruiseControl.IsActive");
    jsonObject.insert("requestId", requestId);

    QJsonDocument jsonDoc(jsonObject);
    return jsonDoc.toJson();
}

QString GetVissTestDataJson::getGetManyJson(QString& requestId)
{
    QJsonObject jsonObject;
    jsonObject.insert("action","get");
    jsonObject.insert("path","Signal.Cabin.Door.*.IsLocked");
    jsonObject.insert("requestId", requestId);

    QJsonDocument jsonDoc(jsonObject);
    return jsonDoc.toJson();
}

QString GetVissTestDataJson::getGetVssJson(QString& requestId)
{
    QJsonObject jsonObject;
    jsonObject.insert("action","getMetadata");
    jsonObject.insert("path","Signal.Drivetrain.Transmission.Speed");
    //jsonObject.insert("path","Signal.ADAS.CruiseControl.IsActive");
    jsonObject.insert("requestId",requestId);

    QJsonDocument jsonDoc(jsonObject);
    return jsonDoc.toJson();
}

QVariant GetVissTestDataJson::getSetValue()
{
    return m_setValue;
}

QString GetVissTestDataJson::getSetJson(QString& requestId)
{
    QJsonObject jsonObject;
    m_setValue = true;
    jsonObject.insert("action","set");
    jsonObject.insert("path","Signal.ADAS.CruiseControl.IsActive");
    jsonObject.insert("value", m_setValue.toBool());
    jsonObject.insert("requestId",requestId);

    QJsonDocument jsonDoc(jsonObject);
    return jsonDoc.toJson();
}

QString GetVissTestDataJson::getSetManyJson(QString& requestId)
{
    QJsonObject jsonObject;

    jsonObject.insert("action","set");
    jsonObject.insert("path","Signal.ADAS.CruiseControl.*");

    QJsonArray values;

    m_setValue = true;
    QJsonObject value1;
    value1.insert("IsActive", m_setValue.toBool());
    values.append(value1);

    m_setValue = 20;
    QJsonObject value2;
    value2.insert("SpeedUp", m_setValue.toInt());
    values.append(value2);

    jsonObject.insert("value", values);
    jsonObject.insert("requestId",requestId);

    QJsonDocument jsonDoc(jsonObject);
    return jsonDoc.toJson();
}

QString GetVissTestDataJson::getUnsubscribe(QString& requestId, QString subscriptionId)
{
    QJsonObject jsonObject;
    jsonObject.insert("action","unsubscribe");
    jsonObject.insert("subscriptionId",subscriptionId);
    jsonObject.insert("requestId", requestId);

    QJsonDocument jsonDoc(jsonObject);
    return jsonDoc.toJson();
}

QString GetVissTestDataJson::getUnsubscribeAll(QString& requestId)
{
    QJsonObject jsonObject;
    jsonObject.insert("action","unsubscribeAll");
    jsonObject.insert("requestId", requestId);

    QJsonDocument jsonDoc(jsonObject);
    return jsonDoc.toJson();
}

// { "action": "authorize", "tokens":{ "authorization": "a-token-value" }, "requestId": "1" }'
QString GetVissTestDataJson::getAuthJson(QString& requestId)
{

    QJsonWebToken l_jwtObj;

    // How should this be distributed in production.
    l_jwtObj.setSecret("mydirtysecret");
    // set a default payload
    l_jwtObj.appendClaim("iss", "MelcoGot");
    l_jwtObj.appendClaim("ValidFrom", QString::number(QDateTime::currentDateTime().toMSecsSinceEpoch()));
    l_jwtObj.appendClaim("ValidTo", QString::number(QDateTime::currentDateTime().addDays(365).toMSecsSinceEpoch()));
    l_jwtObj.appendClaim("path", "Vehicle.*");
    l_jwtObj.appendClaim("actions", "GET,SET,GETVSS,SUBSCRIBE,UNSUBSCRIBE,USUBSCRIBEALL");

    QString zeToken = l_jwtObj.getToken();

    //qDebug() << "Token is: " + zeToken;

    QJsonObject jsonObject;
    jsonObject.insert("action","authorize");

    QJsonObject jsonObject2;
    jsonObject2.insert("authorization",zeToken);
    jsonObject.insert("tokens",jsonObject2);
    jsonObject.insert("requestId", requestId);

    QJsonDocument jsonDoc(jsonObject);
    QString dataJson = jsonDoc.toJson();
    //qDebug() << dataJson;

    return dataJson;
}

QString GetVissTestDataJson::getStatusJson(QString& requestId)
{
    QJsonObject jsonObject;
    jsonObject.insert("action","status");
    jsonObject.insert("requestId", requestId);

    QJsonDocument jsonDoc(jsonObject);
    return jsonDoc.toJson();
}
