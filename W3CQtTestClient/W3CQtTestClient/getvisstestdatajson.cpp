#include "getvisstestdatajson.h"
#include "qjsonwebtoken.h"
#include <QDateTime>
#include <QDebug>
#include <QJsonArray>

GetVissTestDataJson::GetVissTestDataJson()
{

}


QString GetVissTestDataJson::getTestDataString(requesttype type){

    QString testJSON;
    switch(type){
        case requesttype::GET:
            testJSON = "GET";
            break;
        case requesttype::SET:
            testJSON = "SET";
            break;
        case requesttype::SUBSCRIBE:
            testJSON = "SUBSCRIBE";
            break;
        case requesttype::UNSUBSCRIBE:
            testJSON = "UNSUBSCRIBE";
            break;
        case requesttype::UNSUBSCRIBEALL:
            testJSON = "UNSUBSCRIBEALL";
            break;
        case requesttype::GETVSS:
            testJSON = "GETVSS";
            break;
        case requesttype::AUTHORIZE:
            testJSON = getAuthJson();
            break;
    }

    return testJSON;
}


// { "action": "authorize", "tokens":{ "authorization": "a-token-value" }, "requestId": "1" }'
QString GetVissTestDataJson::getAuthJson(){

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

    qDebug() << "Token is: " + zeToken;

    QJsonObject jsonObject;
    jsonObject.insert("action","authorize");

    QJsonObject jsonObject2;
    jsonObject2.insert("authorization",zeToken);
    jsonObject.insert("tokens",jsonObject2);
    jsonObject.insert("requestId","1");

    QJsonDocument jsonDoc(jsonObject);
    QString dataJson = jsonDoc.toJson();
    qDebug() << dataJson;

    return dataJson;
}
