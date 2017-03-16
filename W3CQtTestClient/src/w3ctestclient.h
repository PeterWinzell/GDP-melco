#ifndef W3CTESTCLIENT_H
#define W3CTESTCLIENT_H

#include <QtCore/QObject>
#include <QtWebSockets/QWebSocket>
#include <QtNetwork/QSslError>
#include <QtCore/QList>
#include <QtCore/QQueue>
#include <QtCore/QString>
#include <QtCore/QUrl>

#include "testresult.h"

QT_FORWARD_DECLARE_CLASS(QWebSocket)

Q_DECLARE_METATYPE(TestResult)

class W3cTestClient : public QObject
{
    Q_OBJECT
public:
    explicit W3cTestClient(const QUrl &url, QObject *parent = Q_NULLPTR);
    ~W3cTestClient();

    void RunSubscribeUnsubscribeAllTest();
    void RunSubscribeUnsubscribeTest();
    void RunGetVssTest();
    void RunAuthorizeTest();


    void startClient(QQueue<TestCase> tests);
    void runTest();

signals:
    void testresult(TestResult *result);
    void testsfinished();

private Q_SLOTS:
    void onConnected();
    void onTextMessageReceived(QString message);
    void onSslErrors(const QList<QSslError> &errors);

    //timer slots
    void unsubscribe();
    void unsubscribeAll();
private:
    void passTestRun();
    void failTestRun();



    QString m_unsubscribeCachedSubscriptionId; // keep track of this to perform unsubscribe.
    QWebSocket *m_webSocket = 0;
    QUrl m_url;
    TestCase m_currentTest;
    QQueue<TestCase> m_tests;
    TestResult* m_currentTestResult = 0;
};

#endif // W3CTESTCLIENT_H
