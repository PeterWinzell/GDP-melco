#include "w3ctestclient.h"
#include "getvisstestdatajson.h"
#include <QtCore/QDebug>
#include <QtWebSockets/QWebSocket>
#include <QCoreApplication>
#include <QJsonParseError>
#include <QTime>
#include <QJsonObject>
#include <QTimer>

QT_USE_NAMESPACE

W3cTestClient::W3cTestClient(int clientId, QQueue<TestCase> tests, bool randomize, const QUrl &url, QObject *parent) :
    QObject(parent), m_clientId(clientId), m_tests(tests), m_url(url)
{
    qRegisterMetaType<TestResult>();
    m_clientReport = new ClientReport(m_clientId);
    if(randomize) { std::random_shuffle(tests.begin(), tests.end()); }
}

W3cTestClient::~W3cTestClient()
{
    if(m_webSocket->isValid()) { m_webSocket->close(); }
    m_webSocket->deleteLater();
}

void W3cTestClient::startClient()
{
    if(m_clientStarted) { return; }
    debugOutput("Starting Client");
    m_webSocket = new QWebSocket();

    connect(m_webSocket, &QWebSocket::connected, this, &W3cTestClient::onConnected);
    typedef void (QWebSocket:: *sslErrorsSignal)(const QList<QSslError> &);
    connect(m_webSocket, static_cast<sslErrorsSignal>(&QWebSocket::sslErrors), this, &W3cTestClient::onSslErrors);

    m_webSocket->open(QUrl(m_url));
    m_clientStarted = true;
}

void W3cTestClient::onConnected()
{
    debugOutput("WebSocket connected");
    connect(m_webSocket, &QWebSocket::textMessageReceived,
            this, &W3cTestClient::onTextMessageReceived);

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

                default:
                    break;
            }
        }
        else
        {
            emit testsfinished(m_clientReport);
        }
    }
    else
    {
        debugOutput("Test: " + getTestCaseAsString(m_currentTest) + " pending, waiting...");
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

        default:
            break;
    }

    return caseStr;
}

void W3cTestClient::onTextMessageReceived(QString message)
{
    //qDebug() << "Message received:" << message << "\n";
    //parse message
    QJsonParseError parseError;
    QJsonDocument  jsonDocument;
    QJsonObject    jsonObject;

    jsonDocument = QJsonDocument::fromJson(message.toUtf8(),&parseError);
    jsonObject = jsonDocument.object();
    if (parseError.error == QJsonParseError::NoError)
    {
        QString actionString =jsonObject["action"].toString();

        if (actionString == "subscribe")
        {
            if(m_currentTest != TestCase::SUBSCRIBE_UNSUBSCRIBE && m_currentTest != TestCase::SUBSCRIBEALL_UNSUBSCRIBEALL)
            {
                debugOutput("Received Subcribe action when not requested");

                failTestRun();
                return;
            }

            QString requestId = jsonObject["requestId"].toString();
            QJsonObject errorObject = jsonObject["error"].toObject();
            if (!errorObject.empty())
            {
                debugOutput("Failed to subscribe");
                QString errorMessage = errorObject["message"].toString();
                debugOutput("Error! Request ID [ " + requestId + " ], Message: "+ errorMessage);

                failTestRun();
                return;
            }
            QString path = jsonObject["path"].toString();

            //cache last received subscribeId
            m_unsubscribeCachedSubscriptionId = jsonObject["subscriptionId"].toString();

            debugOutput("Succesfully subscribed to " + path  + ", Sub ID: " + m_unsubscribeCachedSubscriptionId );
        }
        else if (actionString == "unsubscribe")
        {
            if(m_currentTest != TestCase::SUBSCRIBE_UNSUBSCRIBE)
            {
                debugOutput("Received Unsubscribe action when not requested");

                failTestRun();
                return;
            }

            QString  subscriptionId = jsonObject["subscriptionId"].toString();
            QJsonObject errorObject = jsonObject["error"].toObject();
            if (!errorObject.empty())
            {
                QString errorMessage = errorObject["message"].toString();
                debugOutput("Error: " + errorMessage + " SubID: " + subscriptionId);

                failTestRun();
            }

            debugOutput("Succesfully unsuscribed to Sub ID: "+ subscriptionId);

            passTestRun();

        }
        else if (actionString == "unsubscribeAll")
        {
            if(m_currentTest != TestCase::SUBSCRIBEALL_UNSUBSCRIBEALL)
            {
                debugOutput("Received Unsubscribe All action when not requested");

                failTestRun();
                return;
            }

            QString requestId = jsonObject["requestId"].toString();
            QJsonObject errorObject = jsonObject["error"].toObject();
            if (!errorObject.empty())
            {
                debugOutput("Failed to unsubscribe all");
                QString errorMessage = errorObject["message"].toString();
                debugOutput("Error! Request ID [ " + requestId + " ], Message: "+ errorMessage);

                failTestRun();
                return;
            }

            debugOutput("Succesfully performed unsubscribe all");

            passTestRun();
        }
        else if (actionString == "subscribing")
        {
            if(m_currentTest != TestCase::SUBSCRIBE_UNSUBSCRIBE && m_currentTest != TestCase::SUBSCRIBEALL_UNSUBSCRIBEALL)
            {
                debugOutput("Received Subscribing action when not requested");
                failTestRun();
                return;
            }

            QString requestId = jsonObject["requestId"].toString();
            QJsonObject errorObject = jsonObject["error"].toObject();
            if (!errorObject.empty())
            {
                debugOutput("Subscribing failed");
                QString errorMessage = errorObject["message"].toString();
                debugOutput("Error! Request ID [ " + requestId + " ], Message: "+ errorMessage);

                failTestRun();
                return;
            }
            //QString subId = jsonObject["subscriptionId"].toString();
            //QString valueStr  =  jsonObject["value"].toString();
            //int value = valueStr.toInt();
            //QString timeString  = jsonObject["timestamp"].toString();
            //QDateTime time_t = QDateTime::fromTime_t(timeString.toInt());

            //qDebug() << " subscriptionId is : " + subId << " \n";
            //qDebug() << " currentTime is : " + time_t.toString() << " \n";
            //qDebug() << " The value is :   " << value << " \n";

            debugOutput("Received Subscribing message");
            //passTestRun(); //Add a counter of some sort before we pass?
        }
        else if (actionString == "getVSS")
        {
            if(m_currentTest != TestCase::GET_VSS)
            {
                debugOutput("Received Get VSS action when not requested");

                failTestRun();
                return;
            }

            QString requestId = jsonObject["requestId"].toString();
            QJsonObject errorObject = jsonObject["error"].toObject();
            if (!errorObject.empty())
            {
                debugOutput("Failed to get VSS Data");
                QString errorMessage = errorObject["message"].toString();
                debugOutput("Error! Request ID [ " + requestId + " ], Message: "+ errorMessage);

                failTestRun();
                return;
            }

            //QJsonObject vssObject = jsonObject["vss"].toObject();
            debugOutput("VSS data received");
            passTestRun();

        }
        else if (actionString == "authorize")
        {
            if(m_currentTest != TestCase::AUTHORIZE_SUCCESS)
            {
                debugOutput("Received Authorize action when not requested");

                failTestRun();
                return;
            }

            QString requestId = jsonObject["requestId"].toString();
            QJsonObject errorObject = jsonObject["error"].toObject();
            if (!errorObject.empty())
            {
                debugOutput("Failed to authorize");
                QString errorMessage = errorObject["message"].toString();
                debugOutput("Error! Request ID [ " + requestId + " ], Message: "+ errorMessage);

                failTestRun();
                return;
            }

            debugOutput("Successfully authorized");

            passTestRun();

        }
        else if (actionString == "get")
        {
            if((m_currentTest != TestCase::GET) && (m_currentTest != TestCase::SET_GET))
            {
                debugOutput("Received Get action when not requested");
                failTestRun();
                return;
            }

            QString requestId = jsonObject["requestId"].toString();
            QJsonObject errorObject = jsonObject["error"].toObject();
            if (!errorObject.empty())
            {
                debugOutput("Failed to get value");
                QString errorMessage = errorObject["message"].toString();
                debugOutput("Error! Request ID [ " + requestId + " ], Message: "+ errorMessage);

                failTestRun();
                return;
            }

            QString receivedValue = jsonObject["value"].toString();
            QString setValue = GetVissTestDataJson::getSetValue();
            if (receivedValue != setValue)
            {
                debugOutput("Warning! Received value [" +  receivedValue + "] does not match set value [" + setValue + "]");
            }

            debugOutput("Get successfully received."); // Add value to output when test is completly made.
            passTestRun();
        }
        else if (actionString == "set")
        {
            if((m_currentTest != TestCase::SET) && (m_currentTest != TestCase::SET_GET))
            {
                debugOutput("Received Set action when not requested");
                failTestRun();
                return;
            }

            QString requestId = jsonObject["requestId"].toString();
            QJsonObject errorObject = jsonObject["error"].toObject();
            if (!errorObject.empty())
            {
                debugOutput("Failed to set value");
                QString errorMessage = errorObject["message"].toString();
                debugOutput("Error! Request ID [ " + requestId + " ], Message: "+ errorMessage);

                failTestRun();
                return;
            }

            if(m_currentTest == TestCase::SET)
            {
                debugOutput("Set Test Case successful.");
                passTestRun();
            }
            else
            {
                debugOutput("Set response received. Waiting for Get");
            }
        }
        else if (actionString == "status")
        {
            if(m_currentTest != TestCase::STATUS)
            {
                debugOutput("Received Status action when not requested");
                failTestRun();
                return;
            }

            QString requestId = jsonObject["requestId"].toString();
            QJsonObject errorObject = jsonObject["error"].toObject();
            if (!errorObject.empty())
            {
                debugOutput("Failed to get status");
                QString errorMessage = errorObject["message"].toString();
                debugOutput("Error! Request ID [ " + requestId + " ], Message: "+ errorMessage);

                failTestRun();
                return;
            }

            debugOutput("Status Test Case successful.");
            passTestRun();

        }
        else
        {
            debugOutput("Received unknown Request " + actionString);
            failTestRun();
        }
    }
    else
    {
        debugOutput("JSON Parse Error " + parseError.errorString());
        failTestRun();
    }
}


/**
 * @brief W3cTestClient::RunSubscribeUnsubscribeTest
 * Testing subscribe and unsubscribe and unsubscribeall scenario
 */
void W3cTestClient::RunSubscribeUnsubscribeTest()
{
    debugOutput("Running Subscribe + Unsubscribe Test");

    QString subMess = GetVissTestDataJson::getTestDataString(requesttype::SUBSCRIBE);
    m_webSocket->sendTextMessage(subMess);
    QTimer::singleShot(10000,this,SLOT(unsubscribe()));
}

void W3cTestClient::RunSubscribeUnsubscribeAllTest()
{
    debugOutput("Running Subscribe + Unsubscribe All Test");

    QString subMess1 = GetVissTestDataJson::getTestDataString(requesttype::SUBSCRIBE);
    QString subMess2 = GetVissTestDataJson::getTestDataString(requesttype::SUBSCRIBE);
    QString subMess3 = GetVissTestDataJson::getTestDataString(requesttype::SUBSCRIBE);
    /*QString subMess4 = GetVissTestDataJson::getTestDataString(requesttype::SUBSCRIBE);
    QString subMess5 = GetVissTestDataJson::getTestDataString(requesttype::SUBSCRIBE);
    QString subMess6 = GetVissTestDataJson::getTestDataString(requesttype::SUBSCRIBE);
    QString subMess7 = GetVissTestDataJson::getTestDataString(requesttype::SUBSCRIBE);
    QString subMess8 = GetVissTestDataJson::getTestDataString(requesttype::SUBSCRIBE);
    QString subMess9 = GetVissTestDataJson::getTestDataString(requesttype::SUBSCRIBE);
    QString subMess10 = GetVissTestDataJson::getTestDataString(requesttype::SUBSCRIBE);
    QString subMess11= GetVissTestDataJson::getTestDataString(requesttype::SUBSCRIBE);
    QString subMess12 = GetVissTestDataJson::getTestDataString(requesttype::SUBSCRIBE);*/


    //do 3 subscriptions
    m_webSocket->sendTextMessage(subMess1);
    m_webSocket->sendTextMessage(subMess2);
    m_webSocket->sendTextMessage(subMess3);
    /*
        m_webSocket.sendTextMessage(subMess8);
        m_webSocket.sendTextMessage(subMess9);
        m_webSocket.sendTextMessage(subMess10);
        m_webSocket.sendTextMessage(subMess11);
        m_webSocket.sendTextMessage(subMess12);*/



    QTimer::singleShot(60000,this,SLOT(unsubscribeAll()));
}

void W3cTestClient::RunGetVssTest()
{
    debugOutput("Running Get VSS Test");

    QString subMess = GetVissTestDataJson::getTestDataString(requesttype::GETVSS);
    m_webSocket->sendTextMessage(subMess);
}

void W3cTestClient::RunAuthorizeTest()
{
    debugOutput("Running Authorize Test");

    QString dataJson = GetVissTestDataJson::getTestDataString(requesttype::AUTHORIZE);
    m_webSocket->sendTextMessage(dataJson);
}


void W3cTestClient::RunSetGetTest()
{
    debugOutput("Running SetGet Test");

    QString subMess = GetVissTestDataJson::getTestDataString(requesttype::SET);
    m_webSocket->sendTextMessage(subMess);

    QTimer::singleShot(4000,this,SLOT(RunGetTest()));
}

void W3cTestClient::RunSetTest()
{
    debugOutput("Running Set Test");

    QString subMess = GetVissTestDataJson::getTestDataString(requesttype::SET);
    m_webSocket->sendTextMessage(subMess);
}

void W3cTestClient::RunGetTest()
{
    debugOutput("Running Get Test");

    QString subMess = GetVissTestDataJson::getTestDataString(requesttype::GET);
    m_webSocket->sendTextMessage(subMess);
}

void W3cTestClient::RunStatusTest()
{
    debugOutput("Running Status Test");
    QString subMess = GetVissTestDataJson::getTestDataString(requesttype::STATUS);
    m_webSocket->sendTextMessage(subMess);
}


void W3cTestClient::passTestRun()
{
    QHash <QString, QString> finishedTest;
    finishedTest.insert("testcase", QString::number((int)m_currentTest));
    finishedTest.insert("outcome", "passed");
    finishedTest.insert("started", m_testStartTime.toString());
    finishedTest.insert("ended", QDateTime::currentDateTime().toString());

    // Handle special cases here also, if needed

    m_clientReport->m_testResults.append(finishedTest);

    runTest();
}
void W3cTestClient::failTestRun()
{
    QHash <QString, QString> finishedTest;
    finishedTest.insert("testcase", (QString)(int)m_currentTest);
    finishedTest.insert("outcome", "failed");
    finishedTest.insert("started", m_testStartTime.toString());
    finishedTest.insert("ended", QDateTime::currentDateTime().toString());

    debugOutput("Test case failed! Testcase: " + getTestCaseAsString(m_currentTest));

    // Handle special cases here also, if needed.

    m_clientReport->m_testResults.append(finishedTest);

    runTest();
}


void W3cTestClient::unsubscribe()
{
    m_pendingTest = true;
    debugOutput("Unsubscribing " + m_unsubscribeCachedSubscriptionId);
    QString unsubMess = GetVissTestDataJson::getTestDataString(requesttype::UNSUBSCRIBE,m_unsubscribeCachedSubscriptionId);
    m_webSocket->sendTextMessage(unsubMess);

    // wait a while for subscriptions to end
    QTimer::singleShot(2000,this,SLOT(pendingTestTimeout()));
}

void W3cTestClient::unsubscribeAll()
{
    m_pendingTest = true;
    debugOutput("Unsubscribing All");
    QString usubscribeAllMess = GetVissTestDataJson::getTestDataString(requesttype::UNSUBSCRIBEALL);
    m_webSocket->sendTextMessage(usubscribeAllMess);

    // wait a while for subscriptions to end
    QTimer::singleShot(2000,this,SLOT(pendingTestTimeout()));
}

void W3cTestClient::pendingTestTimeout()
{
    m_pendingTest = false;
    debugOutput("Pending test ready, continue with next test.");
    runTest();
}

void W3cTestClient::onSslErrors(const QList<QSslError> &errors)
{
    Q_UNUSED(errors);

    // WARNING: Never ignore SSL errors in production code.
    // The proper way to handle self-signed certificates is to add a custom root
    // to the CA store.

    //foreach( const QSslError &error, errors )
    //{
    //qDebug() << "SSL Error: " << error.errorString();
    //}


    m_webSocket->ignoreSslErrors();
}

void W3cTestClient::debugOutput(QString text)
{
    qDebug() << "[Client#" << m_clientId << "] " << text;
}
