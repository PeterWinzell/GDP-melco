#include "w3ctestclient.h"
#include "getvisstestdatajson.h"
#include <QtWebSockets/QWebSocket>
#include <QCoreApplication>
#include <QJsonParseError>
#include <QTime>
#include <QJsonObject>
#include <QTimer>

#include "logger.h"

QT_USE_NAMESPACE

W3cTestClient::W3cTestClient(int clientId, QQueue<TestCase> tests, bool randomize, const QUrl &url, QObject *parent) :
    QObject(parent), m_clientId(clientId), m_tests(tests), m_url(url), m_testTimeoutSec(60)
{
    qRegisterMetaType<TestResult>();
    m_clientReport = new ClientReport(m_clientId);
    if(randomize) { std::random_shuffle(tests.begin(), tests.end()); }
    TRACE(QString("Client# %1").arg(m_clientId),"< W3cTestClient > created.");

    m_runningTestTimer = new QTimer(this);
    connect(m_runningTestTimer, SIGNAL(timeout()), this, SLOT(testTimeout()));
}

W3cTestClient::~W3cTestClient()
{
    if(m_webSocket->isValid()) { m_webSocket->close(); }
    m_webSocket->deleteLater();
    TRACE(QString("Client# %1").arg(m_clientId),"< W3cTestClient > destroyed.");
}

void W3cTestClient::startClient()
{
    if(m_clientStarted) { return; }
    DEBUG(QString("Client# %1").arg(m_clientId),"Starting client.");
    m_webSocket = new QWebSocket();

    connect(m_webSocket, &QWebSocket::connected, this, &W3cTestClient::onConnected);
    typedef void (QWebSocket:: *sslErrorsSignal)(const QList<QSslError> &);
    connect(m_webSocket, static_cast<sslErrorsSignal>(&QWebSocket::sslErrors), this, &W3cTestClient::onSslErrors);

    m_webSocket->open(QUrl(m_url));
    m_clientStarted = true;
}

void W3cTestClient::onConnected()
{
    DEBUG(QString("Client# %1").arg(m_clientId),"WebSocket connected..");
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

            m_runningTestTimer->setSingleShot(true);
            m_runningTestTimer->start(m_testTimeoutSec * 1000);
        }
        else
        {
            emit testsfinished(m_clientReport);
        }
    }
    else
    {
        TRACE(QString("Client# %1").arg(m_clientId), QString("Test: %1 pending, waiting...").arg(getTestCaseAsString(m_currentTest)));
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
    DEBUG(QString("Client# %1").arg(m_clientId),"Message received");
    TRACE(QString("Client# %1").arg(m_clientId),message);

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
                WARNING(QString("Client# %1").arg(m_clientId),"Received Subcribe action when not requested.");
                failTestRun();
                return;
            }

            QString requestId = jsonObject["requestId"].toString();
            QJsonObject errorObject = jsonObject["error"].toObject();
            if (!errorObject.empty())
            {
                WARNING(QString("Client# %1").arg(m_clientId),"Failed to subscribe.");
                QString errorMessage = errorObject["message"].toString();
                DEBUG(QString("Client# %1").arg(m_clientId),QString("Error! Request ID : %1, Message : %2").arg(requestId, errorMessage));

                failTestRun();
                return;
            }
            QString path = jsonObject["path"].toString();

            //cache last received subscribeId
            m_unsubscribeCachedSubscriptionId = jsonObject["subscriptionId"].toString();

            INFO(QString("Client# %1").arg(m_clientId), QString("Successfully subscribed to signal : %1, sub ID : %2").arg(path, m_unsubscribeCachedSubscriptionId));
        }
        else if (actionString == "unsubscribe")
        {
            if(m_currentTest != TestCase::SUBSCRIBE_UNSUBSCRIBE)
            {
                WARNING(QString("Client# %1").arg(m_clientId),"Received Unsubcribe action when not requested.");
                failTestRun();
                return;
            }

            QString requestId = jsonObject["requestId"].toString();
            QString  subscriptionId = jsonObject["subscriptionId"].toString();
            QJsonObject errorObject = jsonObject["error"].toObject();
            if (!errorObject.empty())
            {
                WARNING(QString("Client# %1").arg(m_clientId),"Unsubscribe failed.");
                QString errorMessage = errorObject["message"].toString();
                DEBUG(QString("Client# %1").arg(m_clientId),QString("Error! Request ID : %1, Message : %2").arg(requestId, errorMessage));

                failTestRun();
                return;
            }
            INFO(QString("Client# %1").arg(m_clientId),"Successfully unsubscribed from Sub ID : " + subscriptionId);

            passTestRun();

        }
        else if (actionString == "unsubscribeAll")
        {
            if(m_currentTest != TestCase::SUBSCRIBEALL_UNSUBSCRIBEALL)
            {
                WARNING(QString("Client# %1").arg(m_clientId),"Received UnsubcribeAll action when not requested.");
                failTestRun();
                return;
            }

            QString requestId = jsonObject["requestId"].toString();
            QJsonObject errorObject = jsonObject["error"].toObject();
            if (!errorObject.empty())
            {
                WARNING(QString("Client# %1").arg(m_clientId),"Unsubscribe all failed.");
                QString errorMessage = errorObject["message"].toString();
                DEBUG(QString("Client# %1").arg(m_clientId),QString("Error! Request ID : %1, Message : %2").arg(requestId, errorMessage));

                failTestRun();
                return;
            }
            INFO(QString("Client# %1").arg(m_clientId),"Successfully unsubscribed from all signals.");

            passTestRun();
        }
        else if (actionString == "subscribing")
        {
            if(m_currentTest != TestCase::SUBSCRIBE_UNSUBSCRIBE && m_currentTest != TestCase::SUBSCRIBEALL_UNSUBSCRIBEALL)
            {
                WARNING(QString("Client# %1").arg(m_clientId),"Received Subscribing action when not requested.");
                failTestRun();
                return;
            }

            QString requestId = jsonObject["requestId"].toString();
            QJsonObject errorObject = jsonObject["error"].toObject();
            if (!errorObject.empty())
            {
                WARNING(QString("Client# %1").arg(m_clientId),"Subscribing failed.");
                QString errorMessage = errorObject["message"].toString();
                DEBUG(QString("Client# %1").arg(m_clientId),QString("Error! Request ID : %1, Message : %2").arg(requestId, errorMessage));

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

            TRACE(QString("Client# %1").arg(m_clientId),"Received Subscribing message.");

            //passTestRun(); //Add a counter of some sort before we pass?
        }
        else if (actionString == "getVSS")
        {
            if(m_currentTest != TestCase::GET_VSS)
            {
                WARNING(QString("Client# %1").arg(m_clientId),"Received GetVSS action when not requested.");
                failTestRun();
                return;
            }

            QString requestId = jsonObject["requestId"].toString();
            QJsonObject errorObject = jsonObject["error"].toObject();
            if (!errorObject.empty())
            {
                WARNING(QString("Client# %1").arg(m_clientId),"Failed to get VSS Data.");
                QString errorMessage = errorObject["message"].toString();
                DEBUG(QString("Client# %1").arg(m_clientId),QString("Error! Request ID : %1, Message : %2").arg(requestId, errorMessage));

                failTestRun();
                return;
            }

            //QJsonObject vssObject = jsonObject["vss"].toObject();
            INFO(QString("Client# %1").arg(m_clientId),"Successfully received VSS Data.");
            TRACE(QString("Client# %1").arg(m_clientId), jsonObject["vss"].toString());
            passTestRun();
        }
        else if (actionString == "authorize")
        {
            if(m_currentTest != TestCase::AUTHORIZE_SUCCESS)
            {
                WARNING(QString("Client# %1").arg(m_clientId),"Received Authorize action when not requested.");
                failTestRun();
                return;
            }

            QString requestId = jsonObject["requestId"].toString();
            QJsonObject errorObject = jsonObject["error"].toObject();
            if (!errorObject.empty())
            {
                WARNING(QString("Client# %1").arg(m_clientId),"Authorize failed.");
                QString errorMessage = errorObject["message"].toString();
                DEBUG(QString("Client# %1").arg(m_clientId),QString("Error! Request ID : %1, Message : %2").arg(requestId, errorMessage));

                failTestRun();
                return;
            }

            INFO(QString("Client# %1").arg(m_clientId),"Successfully authorized.");
            passTestRun();
        }
        else if (actionString == "get")
        {
            if((m_currentTest != TestCase::GET) && (m_currentTest != TestCase::SET_GET))
            {
                WARNING(QString("Client# %1").arg(m_clientId),"Received Get action when not requested.");
                failTestRun();
                return;
            }

            QString requestId = jsonObject["requestId"].toString();
            QJsonObject errorObject = jsonObject["error"].toObject();
            if (!errorObject.empty())
            {
                WARNING(QString("Client# %1").arg(m_clientId),"Get failed.");
                QString errorMessage = errorObject["message"].toString();
                DEBUG(QString("Client# %1").arg(m_clientId),QString("Error! Request ID : %1, Message : %2").arg(requestId, errorMessage));

                failTestRun();
                return;
            }

            QString receivedValue = jsonObject["value"].toString();
            QString setValue = GetVissTestDataJson::getSetValue();
            if (receivedValue != setValue)
            {
                WARNING(QString("Client# %1").arg(m_clientId),QString("Warning! Received value < %1 > does not match set value < %2 >").arg(receivedValue, setValue));
            }

            INFO(QString("Client# %1").arg(m_clientId),"Successfully got value.");
            passTestRun();
        }
        else if (actionString == "set")
        {
            if((m_currentTest != TestCase::SET) && (m_currentTest != TestCase::SET_GET))
            {
                WARNING(QString("Client# %1").arg(m_clientId),"Received Set action when not requested.");
                failTestRun();
                return;
            }

            QString requestId = jsonObject["requestId"].toString();
            QJsonObject errorObject = jsonObject["error"].toObject();
            if (!errorObject.empty())
            {
                WARNING(QString("Client# %1").arg(m_clientId),"Set failed.");
                QString errorMessage = errorObject["message"].toString();
                DEBUG(QString("Client# %1").arg(m_clientId),QString("Error! Request ID : %1, Message : %2").arg(requestId, errorMessage));

                failTestRun();
                return;
            }

            if(m_currentTest == TestCase::SET)
            {
                INFO(QString("Client# %1").arg(m_clientId),"Successfully set value.");
                passTestRun();
            }
            else
            {
                TRACE(QString("Client# %1").arg(m_clientId),"Set response received, waiting for get.");
            }
        }
        else if (actionString == "status")
        {
            if(m_currentTest != TestCase::STATUS)
            {
                WARNING(QString("Client# %1").arg(m_clientId),"Received Status action when not requested.");
                failTestRun();
                return;
            }

            QString requestId = jsonObject["requestId"].toString();
            QJsonObject errorObject = jsonObject["error"].toObject();
            if (!errorObject.empty())
            {
                WARNING(QString("Client# %1").arg(m_clientId),"Failed to receive status.");
                QString errorMessage = errorObject["message"].toString();
                DEBUG(QString("Client# %1").arg(m_clientId),QString("Error! Request ID : %1, Message : %2").arg(requestId, errorMessage));

                failTestRun();
                return;
            }

            INFO(QString("Client# %1").arg(m_clientId),"Successfully received status.");
            passTestRun();
        }
        else
        {
            WARNING(QString("Client# %1").arg(m_clientId),"Received unknown request : " + actionString);
            failTestRun();
        }
    }
    else
    {
        WARNING(QString("Client# %1").arg(m_clientId),"JSON parse error : " + parseError.errorString());
        failTestRun();
    }
}


/**
 * @brief W3cTestClient::RunSubscribeUnsubscribeTest
 * Testing subscribe and unsubscribe and unsubscribeall scenario
 */
void W3cTestClient::RunSubscribeUnsubscribeTest()
{
    INFO(QString("Client# %1").arg(m_clientId),"Running Subscribe + Unsubscribe Test.");

    QString subMess = GetVissTestDataJson::getTestDataString(requesttype::SUBSCRIBE);
    m_webSocket->sendTextMessage(subMess);
    QTimer::singleShot(10000,this,SLOT(unsubscribe()));
}

void W3cTestClient::RunSubscribeUnsubscribeAllTest()
{
    INFO(QString("Client# %1").arg(m_clientId),"Running Subscribe + UnsubscribeAll Test.");

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
    INFO(QString("Client# %1").arg(m_clientId),"Running GetVSS Test.");

    QString subMess = GetVissTestDataJson::getTestDataString(requesttype::GETVSS);
    m_webSocket->sendTextMessage(subMess);
}

void W3cTestClient::RunAuthorizeTest()
{
    INFO(QString("Client# %1").arg(m_clientId),"Running Authorize Test.");

    QString dataJson = GetVissTestDataJson::getTestDataString(requesttype::AUTHORIZE);
    m_webSocket->sendTextMessage(dataJson);
}

void W3cTestClient::RunSetGetTest()
{
    INFO(QString("Client# %1").arg(m_clientId),"Running SetGet Test.");

    QString subMess = GetVissTestDataJson::getTestDataString(requesttype::SET);
    m_webSocket->sendTextMessage(subMess);

    QTimer::singleShot(4000,this,SLOT(RunGetTest()));
}

void W3cTestClient::RunSetTest()
{
    INFO(QString("Client# %1").arg(m_clientId),"Running Set Test.");

    QString subMess = GetVissTestDataJson::getTestDataString(requesttype::SET);
    m_webSocket->sendTextMessage(subMess);
}

void W3cTestClient::RunGetTest()
{
    INFO(QString("Client# %1").arg(m_clientId),"Running Get Test.");

    QString subMess = GetVissTestDataJson::getTestDataString(requesttype::GET);
    m_webSocket->sendTextMessage(subMess);
}

void W3cTestClient::RunStatusTest()
{
    INFO(QString("Client# %1").arg(m_clientId),"Running Status Test.");

    QString subMess = GetVissTestDataJson::getTestDataString(requesttype::STATUS);
    m_webSocket->sendTextMessage(subMess);
}


void W3cTestClient::passTestRun()
{
    m_runningTestTimer->stop();

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

    WARNING(QString("Client# %1").arg(m_clientId),"Test case failed! Testcase: " + getTestCaseAsString(m_currentTest));

    // Handle special cases here also, if needed.

    m_clientReport->m_testResults.append(finishedTest);

    runTest();
}


void W3cTestClient::unsubscribe()
{
    TRACE(QString("Client# %1").arg(m_clientId),"Unsubscribing " + m_unsubscribeCachedSubscriptionId);

    m_pendingTest = true;
    QString unsubMess = GetVissTestDataJson::getTestDataString(requesttype::UNSUBSCRIBE,m_unsubscribeCachedSubscriptionId);
    m_webSocket->sendTextMessage(unsubMess);

    // wait a while for subscriptions to end
    QTimer::singleShot(2000,this,SLOT(pendingTestTimeout()));
}

void W3cTestClient::unsubscribeAll()
{
    TRACE(QString("Client# %1").arg(m_clientId),"Unsubscribing All");

    m_pendingTest = true;
    QString usubscribeAllMess = GetVissTestDataJson::getTestDataString(requesttype::UNSUBSCRIBEALL);
    m_webSocket->sendTextMessage(usubscribeAllMess);

    // wait a while for subscriptions to end
    QTimer::singleShot(2000,this,SLOT(pendingTestTimeout()));
}

void W3cTestClient::pendingTestTimeout()
{
    TRACE(QString("Client# %1").arg(m_clientId),"Pending test ready, continue with next test.");

    m_pendingTest = false;
    runTest();
}

void W3cTestClient::testTimeout()
{
    WARNING(QString("Client# %1").arg(m_clientId), QString("No response from server! Waited %1 seconds").arg(m_testTimeoutSec));
    failTestRun();
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
