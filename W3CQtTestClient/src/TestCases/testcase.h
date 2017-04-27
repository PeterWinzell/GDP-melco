#ifndef TESTCASE_H
#define TESTCASE_H

#include <QObject>
#include <QWebSocket>
#include <QString>
#include "logger.h"
#include "getvisstestdatajson.h"

enum class TestCase
{
    UNKNOWN,
    SUBSCRIBE_UNSUBSCRIBE,
    SUBSCRIBEALL_UNSUBSCRIBEALL,
    AUTHORIZE_SUCCESS,
    GET_VSS,
    SET_GET,
    SET,
    GET,
    STATUS,
    GET_MANY,
    SET_MANY,
    NR_OF_TESTCASES
};

class TestCaseBase : public QObject
{
    Q_OBJECT
public:
    explicit TestCaseBase(QString testClientId, int& requestid,QObject *parent = NULL): m_requestId(requestid), m_testClientId(testClientId), QObject(parent) {}
    virtual void startTest(QWebSocket* webSocket) = 0;
    TestCase getType() { return m_testCaseType; }

signals:
    void finished(bool result);

public slots:
    virtual void forceStop() = 0;
    virtual void onTextMessageReceived(QString message) = 0;

protected:
    TestCase m_testCaseType;
    int& m_requestId;
    QWebSocket* m_webSocket;
    QString m_testClientId;
};

#endif // TESTCASE_H
