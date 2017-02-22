#ifndef VISSREQUEST_H
#define VISSREQUEST_H

#include <QString>
#include <QVariant>
#include <QJsonObject>
#include <QDebug>

// What is the best way to represent these? enum with int or str? macro?
enum vss_request
{
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
    vss_request m_action;

    QString m_requestId;
    QString m_signalPath;

    QVariant m_value;

    QString m_subscriptionId;
    QVariant m_filters;

    QVariant m_tokens;

    QJsonObject m_jsonObject;


};

#endif // VISSREQUEST_H
