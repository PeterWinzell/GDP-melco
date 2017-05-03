#ifndef GETVISSTESTDATAJSON_H
#define GETVISSTESTDATAJSON_H
#include <QString>
#include <QVariant>

enum requesttype {GET=1,SET,SUBSCRIBE,UNSUBSCRIBE,UNSUBSCRIBEALL,GETVSS,AUTHORIZE,STATUS,GET_MANY, SET_MANY};

class GetVissTestDataJson
{
public:
    GetVissTestDataJson();
    static QString getTestDataString(requesttype type, QString requestId, QString subId = "1");

    // return a JSON {"action":"authorize...}
    static QString getAuthJson(QString& requestId);
    static QString getSubscriptionJson(QString& requestId);
    static QString getUnsubscribe(QString& requestId, QString subscriptionId);
    static QString getUnsubscribeAll(QString& requestId);
    static QString getGetJson(QString& requestId);
    static QString getGetManyJson(QString& requestId);
    static QString getSetJson(QString& requestId);
    static QString getSetManyJson(QString& requestId);
    static QString getGetVssJson(QString& requestId);
    static QVariant getSetValue();
    static QString getStatusJson(QString& requestId);
private:
    static QVariant m_setValue;
};

#endif // GETVISSTESTDATAJSON_H
