#ifndef SUBSCRIBEUNSUBSCRIBEALLTESTCASE_H
#define SUBSCRIBEUNSUBSCRIBEALLTESTCASE_H

#include <QObject>
#include <TestCases/testcasebase.h>

class SubscribeUnsubscribeAllTestCase : public TestCaseBase
{
    Q_OBJECT
public:
    SubscribeUnsubscribeAllTestCase(QString testClientId, int& requestid, QObject *parent = NULL);
    void startTest(QWebSocket* webSocket);

signals:
    void finished(bool result);

public slots:
    void forceStop();
    void onTextMessageReceived(QString message);

private slots:
    void unsubscribeAll();
    void pendingTestTimeout();
private:
    QString m_unsubscribeCachedSubscriptionId;
};

#endif // SUBSCRIBEUNSUBSCRIBEALLTESTCASE_H
