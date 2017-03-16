#include "w3ctestclienthandler.h"
#include <QDebug>
#include <QThread>

W3cTestClientHandler::W3cTestClientHandler(int nrOfClients, QQueue<TestCase> tests, bool randomize, bool secure, QString url )
{
    qRegisterMetaType<QQueue<TestCase>>();

    for(int i = 0; i < nrOfClients; i++)
    {
        QThread* clientThread = new QThread();

        W3cTestClient* client = new W3cTestClient(QUrl(url));
        client->moveToThread(clientThread);

        connect(this,           &W3cTestClientHandler::startClients,
                client,         &W3cTestClient::startClient);           // Start Client

        connect(client,         &W3cTestClient::testresult,
                this,           &W3cTestClientHandler::handleTestResult);   // Get Result from Client

        connect(client,         &W3cTestClient::testsfinished,
                this,           &W3cTestClientHandler::handleTestClientCompletion);   // When all Tests are done on Client

        connect(client,         &W3cTestClient::testsfinished,
                client,         &QObject::deleteLater);                 // When done, delete Client

        connect(client,         &W3cTestClient::testsfinished,
                clientThread,   &QThread::quit);                        // When done, quit thread

        connect(clientThread,   &QThread::finished,
                clientThread,   &QObject::deleteLater);                 // When thread done, delete Thread

        clientThread->start();
        m_clients.append(clientThread);

        emit startClients(tests);
    }
    //emit startClients(test); // If we want all to run at the same time, could be toublesome if we got many clients.
}

W3cTestClientHandler::~W3cTestClientHandler()
{

}

void W3cTestClientHandler::handleTestResult(TestResult* result)
{
    qDebug() << " Test Finished!";
}
void W3cTestClientHandler::handleTestClientCompletion()
{
    qDebug() << " Test Client Finished!";
}
