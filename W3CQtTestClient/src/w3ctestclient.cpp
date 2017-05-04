#include "w3ctestclient.h"
#include "getvisstestdatajson.h"
#include <QtWebSockets/QWebSocket>
#include <QCoreApplication>
#include <QJsonParseError>
#include <QTime>
#include <QJsonObject>
#include <QTimer>

#include "logger.h"
#include "TestCases/gettestcase.h"
#include "TestCases/settestcase.h"
#include "TestCases/subscribeunsubscribetestcase.h"
#include "TestCases/subscribeunsubscribealltestcase.h"
#include "TestCases/getvsstestcase.h"
#include "TestCases/authorizetestcase.h"
#include "TestCases/getmanytestcase.h"
#include "TestCases/setmanytestcase.h"
#include "TestCases/statustestcase.h"
#include "TestCases/setgettestcase.h"

QT_USE_NAMESPACE

W3cTestClient::W3cTestClient(int clientId, QQueue<TestCase> tests, bool randomize, const QUrl &url, QObject *parent) :
    QObject(parent), m_clientId(clientId), m_clientIdStr(QString("Client# %1").arg(m_clientId)), m_url(url),m_tests(tests),  m_testTimeoutSec(60)
{
    qRegisterMetaType<TestResult>();
    m_clientReport = new ClientReport(m_clientId);
    if(randomize) { std::random_shuffle(tests.begin(), tests.end()); }
    TRACE(m_clientIdStr,"< W3cTestClient > created.");

    m_runningTestTimer = new QTimer(this);
    connect(m_runningTestTimer, SIGNAL(timeout()), this, SLOT(testTimeout()));
}

W3cTestClient::~W3cTestClient()
{
    if(m_webSocket->isValid()) { m_webSocket->close(); }
    m_webSocket->deleteLater();
    TRACE(m_clientIdStr,"< W3cTestClient > destroyed.");
}

void W3cTestClient::startClient()
{
    if(m_clientStarted) { return; }
    INFO(m_clientIdStr, QString("Starting client. Connecting to %1 ...").arg(m_url.toString()));
    m_webSocket = new QWebSocket();

    connect(m_webSocket, &QWebSocket::connected, this, &W3cTestClient::onConnected);
    typedef void (QWebSocket:: *sslErrorsSignal)(const QList<QSslError> &);
    connect(m_webSocket, static_cast<sslErrorsSignal>(&QWebSocket::sslErrors), this, &W3cTestClient::onSslErrors);

    connect(m_webSocket, static_cast<void(QWebSocket::*)(QAbstractSocket::SocketError)>(&QWebSocket::error),
            [=]()
    {
        WARNING(m_clientIdStr, QString("Socket error: %1").arg(m_webSocket->errorString()));
        QCoreApplication::exit(-1);
    });
    // Connect for 10 seconds, then abort and fail client.
    QTimer::singleShot(10000, this, SLOT(connectionTimeout()));

    m_webSocket->open(QUrl(m_url));
    m_clientStarted = true;
}

void W3cTestClient::onConnected()
{
    INFO(m_clientIdStr,"WebSocket connected..");
    //connect(m_webSocket, &QWebSocket::textMessageReceived, this, &W3cTestClient::onTextMessageReceived);

    m_currentTest = TestCase::UNKNOWN;
    m_pendingTest = false;

    runTest();
}

void W3cTestClient::runTest()
{
    if(!m_pendingTest)
    {
        if (m_tests.length() > 0)
        {
            m_currentTest = m_tests.dequeue();
            m_testStartTime = QDateTime::currentDateTime();

            switch (m_currentTest)
            {
                case TestCase::SUBSCRIBE_UNSUBSCRIBE:
                    RunSubscribeUnsubscribeTest();
                    break;

                case TestCase::SUBSCRIBEALL_UNSUBSCRIBEALL:
                    RunSubscribeUnsubscribeAllTest();
                    break;

                case TestCase::GET_VSS:
                    RunGetVssTest();
                    break;

                case TestCase::SET_GET:
                    RunSetGetTest();
                    break;

                case TestCase::SET:
                    RunSetTest();
                    break;

                case TestCase::GET:
                    RunGetTest();
                    break;

                case TestCase::AUTHORIZE_SUCCESS:
                    RunAuthorizeTest();
                    break;

                case TestCase::STATUS:
                    RunStatusTest();
                    break;

                case TestCase::GET_MANY:
                    RunGetManyTest();
                    break;

                case TestCase::SET_MANY:
                    RunSetManyTest();
                    break;
                default:
                    break;
            }

            m_runningTestTimer->setSingleShot(true);
            m_runningTestTimer->start(m_testTimeoutSec * 1000);
        }
        else
        {
            m_webSocket->close();
            emit testsfinished(m_clientReport);
        }
    }
    else
    {
        TRACE(m_clientIdStr, QString("Test: %1 pending, waiting...").arg(getTestCaseAsString(m_currentTest)));
    }
}

QString W3cTestClient::getTestCaseAsString(TestCase testCase)
{
    QString caseStr = "UNKNOWN";

    switch (testCase)
    {
        case TestCase::SUBSCRIBE_UNSUBSCRIBE:
            caseStr = "SUBSCRIBE_UNSUBSCRIBE";
            break;

        case TestCase::SUBSCRIBEALL_UNSUBSCRIBEALL:
            caseStr = "SUBSCRIBEALL_UNSUBSCRIBEALL";
            break;

        case TestCase::GET_VSS:
            caseStr = "GET_VSS";
            break;

        case TestCase::SET_GET:
            caseStr = "SET_GET";
            break;

        case TestCase::SET:
            caseStr = "SET";
            break;

        case TestCase::GET:
            caseStr = "GET";
            break;

        case TestCase::AUTHORIZE_SUCCESS:
            caseStr = "AUTHORIZE_SUCCESS";
            break;

        case TestCase::STATUS:
            caseStr = "STATUS";
            break;

        case TestCase::GET_MANY:
            caseStr = "GET_MANY";
            break;

        case TestCase::SET_MANY:
            caseStr = "SET_MANY";
            break;

        default:
            break;
    }

    return caseStr;
}

/**
 * @brief W3cTestClient::RunSubscribeUnsubscribeTest
 * Testing subscribe and unsubscribe and unsubscribeall scenario
 */
void W3cTestClient::RunSubscribeUnsubscribeTest()
{
    INFO(m_clientIdStr,"Running Subscribe + Unsubscribe Test.");
    m_currentRunningTest = new SubscribeUnsubscribeTestCase(m_clientIdStr, m_requestId);

    connect(static_cast<SubscribeUnsubscribeTestCase*>(m_currentRunningTest), &SubscribeUnsubscribeTestCase::finished, this, &W3cTestClient::onTestFinished);

    m_currentRunningTest->startTest(m_webSocket);
}

void W3cTestClient::RunSubscribeUnsubscribeAllTest()
{
    INFO(m_clientIdStr,"Running Subscribe + UnsubscribeAll Test.");
    m_currentRunningTest = new SubscribeUnsubscribeAllTestCase(m_clientIdStr, m_requestId);

    connect(static_cast<SubscribeUnsubscribeAllTestCase*>(m_currentRunningTest), &SubscribeUnsubscribeAllTestCase::finished, this, &W3cTestClient::onTestFinished);

    m_currentRunningTest->startTest(m_webSocket);
}

void W3cTestClient::RunGetVssTest()
{
    INFO(m_clientIdStr,"Running GetVSS Test.");
    m_currentRunningTest = new GetVSSTestCase(m_clientIdStr, m_requestId);

    connect(static_cast<GetVSSTestCase*>(m_currentRunningTest), &GetVSSTestCase::finished, this, &W3cTestClient::onTestFinished);

    m_currentRunningTest->startTest(m_webSocket);
}

void W3cTestClient::RunAuthorizeTest()
{
    INFO(m_clientIdStr,"Running Authorize Test.");
    m_currentRunningTest = new AuthorizeTestCase(m_clientIdStr, m_requestId);

    connect(static_cast<AuthorizeTestCase*>(m_currentRunningTest), &AuthorizeTestCase::finished, this, &W3cTestClient::onTestFinished);

    m_currentRunningTest->startTest(m_webSocket);
}

void W3cTestClient::RunSetGetTest()
{
    INFO(m_clientIdStr,"Running SetGet Test.");
    m_currentRunningTest = new SetGetTestCase(m_clientIdStr, m_requestId);

    connect(static_cast<SetGetTestCase*>(m_currentRunningTest), &SetGetTestCase::finished, this, &W3cTestClient::onTestFinished);

    m_currentRunningTest->startTest(m_webSocket);

}

void W3cTestClient::RunSetTest()
{
    INFO(m_clientIdStr,"Running Set Test.");
    m_currentRunningTest = new SetTestCase(m_clientIdStr, m_requestId);

    connect(static_cast<SetTestCase*>(m_currentRunningTest), &SetTestCase::finished, this, &W3cTestClient::onTestFinished);

    m_currentRunningTest->startTest(m_webSocket);
}

void W3cTestClient::RunGetTest()
{
    INFO(m_clientIdStr,"Running Get Test.");
    m_currentRunningTest = new GetTestCase(m_clientIdStr, m_requestId);

    connect(static_cast<GetTestCase*>(m_currentRunningTest), &GetTestCase::finished, this, &W3cTestClient::onTestFinished);

    m_currentRunningTest->startTest(m_webSocket);
}

void W3cTestClient::RunStatusTest()
{
    INFO(m_clientIdStr,"Running Status Test.");
    m_currentRunningTest = new StatusTestCase(m_clientIdStr, m_requestId);

    connect(static_cast<StatusTestCase*>(m_currentRunningTest), &StatusTestCase::finished, this, &W3cTestClient::onTestFinished);

    m_currentRunningTest->startTest(m_webSocket);
}

void W3cTestClient::RunGetManyTest()
{
    INFO(m_clientIdStr,"Running Get Many Test.");
    m_currentRunningTest = new GetManyTestCase(m_clientIdStr, m_requestId);

    connect(static_cast<GetManyTestCase*>(m_currentRunningTest), &GetManyTestCase::finished, this, &W3cTestClient::onTestFinished);

    m_currentRunningTest->startTest(m_webSocket);
}

void W3cTestClient::RunSetManyTest()
{
    INFO(m_clientIdStr,"Running Set Many Test.");
    m_currentRunningTest = new SetManyTestCase(m_clientIdStr, m_requestId);

    connect(static_cast<SetManyTestCase*>(m_currentRunningTest), &SetManyTestCase::finished, this, &W3cTestClient::onTestFinished);

    m_currentRunningTest->startTest(m_webSocket);
}

void W3cTestClient::onTestFinished(bool success)
{
    m_runningTestTimer->stop();
    disconnect(m_currentRunningTest,0,0,0);
    m_currentRunningTest->deleteLater();

    QHash <QString, QString> finishedTest;
    finishedTest.insert("testcase", QString::number((int)m_currentTest));
    finishedTest.insert("outcome", success ? "passed" : "failed");
    finishedTest.insert("started", m_testStartTime.toString());
    finishedTest.insert("ended", QDateTime::currentDateTime().toString());

    // Handle special cases here also, if needed

    m_clientReport->m_testResults.append(finishedTest);

    runTest();
}

void W3cTestClient::testTimeout()
{
    WARNING(m_clientIdStr, QString("No response from server! Waited %1 seconds").arg(m_testTimeoutSec));
    onTestFinished(false);
}

void W3cTestClient::connectionTimeout()
{
    if(m_webSocket->isValid()){ return; }
    CRITICAL(m_clientIdStr,"Could not connect to Server.");
    onTestFinished(false);
}

void W3cTestClient::onSslErrors(const QList<QSslError> &errors)
{
    Q_UNUSED(errors);

    // WARNING: Never ignore SSL errors in production code.
    // The proper way to handle self-signed certificates is to add a custom root
    // to the CA store.

    foreach( const QSslError &error, errors )
    {
        WARNING(m_clientIdStr, QString("SSL Error: %1").arg(error.errorString()));
    }


    m_webSocket->ignoreSslErrors();
}
