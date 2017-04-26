#ifndef W3CTESTCLIENT_H
#define W3CTESTCLIENT_H

#include <QtCore/QObject>
#include <QtWebSockets/QWebSocket>
#include <QtNetwork/QSslError>
#include <QtCore/QList>
#include <QtCore/QQueue>
#include <QtCore/QString>
#include <QtCore/QUrl>
#include <QTimer>

#include "testresult.h"
#include "clientreport.h"

QT_FORWARD_DECLARE_CLASS(QWebSocket)

Q_DECLARE_METATYPE(TestResult)

class W3cTestClient : public QObject
{
    Q_OBJECT
public:
    explicit W3cTestClient(int clientId, QQueue<TestCase> tests, bool randomize, const QUrl &url, QObject *parent = Q_NULLPTR);
    ~W3cTestClient();

    void RunSubscribeUnsubscribeAllTest();
    void RunSubscribeUnsubscribeTest();
    void RunGetVssTest();
    void RunSetGetTest();
    void RunSetTest();
    void RunAuthorizeTest();
    void RunStatusTest();
    void RunGetManyTest();
    void RunSetManyTest();

    void startClient();
    void runTest();

signals:
    void testresult(TestResult *result);
    void testsfinished(ClientReport *m_currentRunningTest);

private Q_SLOTS:
    void onConnected();
    void onTextMessageReceived(QString message);
    void onSslErrors(const QList<QSslError> &errors);

    //timer slots
    void RunGetTest();
    void unsubscribe();
    void unsubscribeAll();
    void pendingTestTimeout();
    void testTimeout();

private:
    void passTestRun(bool success);

    QString getTestCaseAsString(TestCase testCase);

    bool m_clientStarted = false;
    ClientReport *m_clientReport;

    QString m_unsubscribeCachedSubscriptionId; // keep track of this to perform unsubscribe.
    QWebSocket *m_webSocket = 0;

    int m_clientId;
    int m_requestId = 0;
    QUrl m_url;

    QQueue<TestCase> m_tests;
    TestCase m_currentTest;
    QDateTime m_testStartTime;
    const int m_testTimeoutSec;
    bool m_pendingTest;

    QTimer* m_runningTestTimer;

};

#endif // W3CTESTCLIENT_H
