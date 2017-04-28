#ifndef TESTCASEBASE_H
#define TESTCASEBASE_H

#include <QObject>
#include <QWebSocket>
#include <QString>
#include "logger.h"
#include "getvisstestdatajson.h"
#include "testcase.h"

class TestCaseBase : public QObject
{
    Q_OBJECT
public:
    explicit TestCaseBase(QString testClientId, int& requestid,QObject *parent = NULL): QObject(parent), m_testClientId(testClientId), m_requestId(requestid) {}
    virtual void startTest(QWebSocket* webSocket) = 0;
    TestCase getType() { return m_testCaseType; }

signals:
    void finished(bool result);

public slots:
    virtual void forceStop() = 0;
    virtual void onTextMessageReceived(QString message) = 0;

protected:
    QString m_testClientId;
    int& m_requestId;
    TestCase m_testCaseType;

    QWebSocket* m_webSocket;
};

#endif // TESTCASEBASE_H
