#ifndef GETVISSTESTDATAJSON_H
#define GETVISSTESTDATAJSON_H
#include <QString>

enum requesttype {GET=1,SET,SUBSCRIBE,UNSUBSCRIBE,UNSUBSCRIBEALL,GETVSS,AUTHORIZE};

class GetVissTestDataJson
{
public:
    GetVissTestDataJson();
    static QString getTestDataString(requesttype type,QString subId = "1");

    // return a JSON {"action":"authorize...}
    static QString getAuthJson();
    static QString getSubscriptionJson();
    static QString getUnsubscribe(QString subscriptionId);
    static QString getUnsubscribeAll();
    static QString getGetJson();
    static QString getSetJson();
    static QString getGetVssJson();

    static int m_requestId;
};

#endif // GETVISSTESTDATAJSON_H
