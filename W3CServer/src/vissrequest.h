#ifndef VISSREQUEST_H
#define VISSREQUEST_H

#include <QString>
#include <QVariant>
#include <QJsonObject>
#include <QJsonDocument>
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
    GETMETADATA,
    STATUS,
    ERROR,
};

class VISSRequest : public QObject
{
public:
    VISSRequest(QString json, QObject *parent = Q_NULLPTR) : QObject(parent) { m_jsonString = json; m_action = ERROR; }

    QString getJsonString() {return m_jsonString; }
    QJsonDocument getJsonDocument() { return m_jsonDocument; }
    QJsonObject getJsonObject() { return m_jsonObject; }

    void setJsonDocument(QJsonDocument jsonDocument) {m_jsonDocument = jsonDocument; }
    void setJsonObject(QJsonObject jsonObject) {m_jsonObject = jsonObject; }

    void invalidateRequest() { m_action = ERROR; }

    vss_request getAction() {return m_action;}
    QString  getRequestId() {return m_requestId;}
    QString  getSignalPath() {return m_signalPath;}
    QVariant getValue() {return m_value;}
    QString  getSubscriptionId() {return m_subscriptionId;}
    QVariant getFilters() {return m_filters;}
    QVariant getTokens() {return m_tokens;}

    void setAction(vss_request action) {m_action = action; }
    void setRequestId(QString requestId) {m_requestId = requestId; }
    void setSignalPath(QString signalPath) {m_signalPath = signalPath; }
    void setValue(QVariant value) {m_value = value; }
    void setSubscriptionId(QString subscriptionId) {m_subscriptionId = subscriptionId; }
    void setFilters(QVariant filters) {m_filters = filters; }
    void setTokens(QVariant tokens) {m_tokens = tokens; }

private:

    QString m_jsonString;
    QJsonDocument m_jsonDocument;
    QJsonObject m_jsonObject;

    vss_request m_action;
    QString m_requestId;
    QString m_signalPath;
    QVariant m_value;
    QString m_subscriptionId;
    QVariant m_filters;
    QVariant m_tokens;

};

#endif // VISSREQUEST_H
