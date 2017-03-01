#ifndef GETVISSTESTDATAJSON_H
#define GETVISSTESTDATAJSON_H
#include <QString>

enum requesttype {GET=1,SET,SUBSCRIBE,UNSUBSCRIBE,UNSUBSCRIBEALL,GETVSS,AUTHORIZE};

class GetVissTestDataJson
{
public:
    GetVissTestDataJson();
    static QString getTestDataString(requesttype type);

    // return a JSON {"action":"authorize...}
    static QString getAuthJson();
    static QString getSubscriptionJson();
    static QString getUnsubscribe(int subscriptionId);
    static QString getUnsubscribeAll();
};

#endif // GETVISSTESTDATAJSON_H
