#include "getvisstestdatajson.h"
#include "qjsonwebtoken.h"
#include <QDateTime>
#include <QDebug>
#include <QJsonArray>

int GetVissTestDataJson::m_requestId = 0;
QString GetVissTestDataJson::m_setValue = "0";

GetVissTestDataJson::GetVissTestDataJson()
{

}


QString GetVissTestDataJson::getTestDataString(requesttype type, QString subId)
{

    QString testJSON;
    m_requestId++;
    switch(type)
    {
        case requesttype::GET:
            testJSON = getGetJson();
            break;
        case requesttype::SET:
            testJSON = getSetJson();
            break;
        case requesttype::SUBSCRIBE:
            testJSON = getSubscriptionJson();
            break;
        case requesttype::UNSUBSCRIBE:
            testJSON = getUnsubscribe(subId);
            break;
        case requesttype::UNSUBSCRIBEALL:
            testJSON = getUnsubscribeAll();
            break;
        case requesttype::GETVSS:
            testJSON = getGetVssJson();
            break;
        case requesttype::AUTHORIZE:
            testJSON = getAuthJson();
            break;
        case requesttype::STATUS:
            testJSON = getStatusJson();
            break;
    }

    return testJSON;
}

QString GetVissTestDataJson::getSubscriptionJson()
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

    jsonObject.insert("requestId","1");
    jsonObject.insert("timestamp", QString::number(QDateTime::currentDateTime().toTime_t() ));

    QJsonDocument jsonDoc(jsonObject);
    return jsonDoc.toJson();
}

QString GetVissTestDataJson::getGetJson()
{
    QJsonObject jsonObject;
    jsonObject.insert("action","get");
    jsonObject.insert("path","Signal.Drivetrain.Transmission.Speed");
    jsonObject.insert("requestId","1");

    QJsonDocument jsonDoc(jsonObject);
    return jsonDoc.toJson();
}

QString GetVissTestDataJson::getGetVssJson()
{
    QJsonObject jsonObject;
    jsonObject.insert("action","getVSS");
    jsonObject.insert("path","Signal.Drivetrain.Transmission.Speed");
    jsonObject.insert("requestId","1");

    QJsonDocument jsonDoc(jsonObject);
    return jsonDoc.toJson();
}

QString GetVissTestDataJson::getSetValue()
{
    return m_setValue;
}

QString GetVissTestDataJson::getSetJson()
{
    QJsonObject jsonObject;
    m_setValue = "90";
    jsonObject.insert("action","set");
    jsonObject.insert("path","Signal.Drivetrain.Transmission.Speed");
    jsonObject.insert("value", m_setValue);
    jsonObject.insert("requestId","1");

    QJsonDocument jsonDoc(jsonObject);
    return jsonDoc.toJson();
}

QString GetVissTestDataJson::getUnsubscribe(QString subscriptionId)
{
    QJsonObject jsonObject;
    jsonObject.insert("action","unsubscribe");
    jsonObject.insert("subscriptionId",subscriptionId);
    jsonObject.insert("requestId",QString::number(m_requestId));

    QJsonDocument jsonDoc(jsonObject);
    return jsonDoc.toJson();
}

QString GetVissTestDataJson::getUnsubscribeAll()
{
    QJsonObject jsonObject;
    jsonObject.insert("action","unsubscribeAll");
    jsonObject.insert("requestId",QString::number(m_requestId));

    QJsonDocument jsonDoc(jsonObject);
    return jsonDoc.toJson();
}

// { "action": "authorize", "tokens":{ "authorization": "a-token-value" }, "requestId": "1" }'
QString GetVissTestDataJson::getAuthJson()
{

    QJsonWebToken l_jwtObj;

    // How should this be distributed in production.
    l_jwtObj.setSecret("mydirtysecret");
    // set a default payload
    l_jwtObj.appendClaim("iss", "MelcoGot");
    l_jwtObj.appendClaim("ValidFrom", QString::number(QDateTime::currentDateTime().toTime_t()));
    l_jwtObj.appendClaim("ValidTo", QString::number(QDateTime::currentDateTime().addDays(365).toTime_t()));
    l_jwtObj.appendClaim("path", "Vehicle.*");
    l_jwtObj.appendClaim("actions", "GET,SET,GETVSS,SUBSCRIBE,UNSUBSCRIBE,USUBSCRIBEALL");

    QString zeToken = l_jwtObj.getToken();

    //qDebug() << "Token is: " + zeToken;

    QJsonObject jsonObject;
    jsonObject.insert("action","authorize");

    QJsonObject jsonObject2;
    jsonObject2.insert("authorization",zeToken);
    jsonObject.insert("tokens",jsonObject2);
    jsonObject.insert("requestId",QString::number(m_requestId));

    QJsonDocument jsonDoc(jsonObject);
    QString dataJson = jsonDoc.toJson();
    //qDebug() << dataJson;

    return dataJson;
}

QString GetVissTestDataJson::getStatusJson()
{
    QJsonObject jsonObject;
    m_setValue = "90";
    jsonObject.insert("action","status");
    jsonObject.insert("requestId","1");

    QJsonDocument jsonDoc(jsonObject);
    return jsonDoc.toJson();
}
