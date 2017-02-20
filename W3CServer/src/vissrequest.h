#ifndef VISSREQUEST_H
#define VISSREQUEST_H

#include <QString>
#include <QVariant>
#include <QJsonObject>
#include <QDebug>

// What is the best way to represent these? enum with int or str? macro?
enum vss_request{
    GET,
    SET,
    SUBSCRIBE,
    UNSUBSCRIBE,
    UNSUBSCRIBEALL,
    AUTHORIZE,
    GETVSS,
    ERROR
};

class VISSRequest
{
public:
    VISSRequest();
    VISSRequest(QJsonObject& json);

    QJsonObject getJsonObject();

    void invalidateRequest();

    vss_request getAction();
    QString  getRequestId();
    QString  getSignalPath();

    QVariant getValue();

    QString  getSubscriptionId();
    QVariant getFilters();

    QVariant getTokens();

    void setRequestValues();

private:
    vss_request action;

    QString requestId;
    QString signalPath;

    QVariant value;

    QString subscriptionId;
    QVariant filters;

    QVariant tokens;

    QJsonObject jsonObject;


};

#endif // VISSREQUEST_H
