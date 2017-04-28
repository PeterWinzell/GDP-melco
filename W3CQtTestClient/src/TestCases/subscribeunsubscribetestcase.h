#ifndef SUBSCRIBEUNSUBSCRIBETESTCASE_H
#define SUBSCRIBEUNSUBSCRIBETESTCASE_H

#include <QObject>
#include "TestCases/testcasebase.h"

class SubscribeUnsubscribeTestCase : public TestCaseBase
{
    Q_OBJECT
public:
    SubscribeUnsubscribeTestCase(QString testClientId,int& requestid, QObject *parent = NULL);
    void startTest(QWebSocket* webSocket);

signals:
    void finished(bool result);

public slots:
    void forceStop();
    void onTextMessageReceived(QString message);

private slots:
    void unsubscribe();
    void pendingTestTimeout();
private:
    QString m_unsubscribeCachedSubscriptionId;
};

#endif // SUBSCRIBEUNSUBSCRIBETESTCASE_H
