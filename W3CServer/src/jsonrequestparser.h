#ifndef JSONREQUESTPARSER_H
#define JSONREQUESTPARSER_H


#include <QString>
#include <QJsonObject>
#include <QJsonParseError>
#include <QDebug>
#include <QRegExp>
#include <QSharedPointer>

#include "vissrequest.h"

class JSONRequestParser : public QObject
{
public:
    JSONRequestParser(bool debug = false, QObject *parent = Q_NULLPTR);
    ~JSONRequestParser();
    QSharedPointer<VISSRequest> parseJson(QString json);
private:
    bool m_debug;
    bool isValidJson();

    bool validateGetRequest(VISSRequest* request);
    bool validateSetRequest(VISSRequest* request);
    bool validateSubscribeRequest(VISSRequest* request);
    bool validateUnsubscribeRequest(VISSRequest* request);
    bool validateUnsubscribeAllRequest(VISSRequest* request);
    bool validateAuthorizeRequest(VISSRequest* request);
    bool validateGetMetadataRequest(VISSRequest* request);
    bool validateStatusRequest(VISSRequest* request);

    bool validateAction(VISSRequest* request);
    bool validatePath(QJsonValue value);
    bool validateId(QJsonValue value);

    bool validateValue(QJsonValue value);
    bool validateSubscriptionId(QJsonValue value);
    bool validateFilters(QJsonValue value);
    bool validateTokens(QJsonValue value);

    QJsonParseError m_parseError;
};
#endif // JSONREQUESTPARSER_H
