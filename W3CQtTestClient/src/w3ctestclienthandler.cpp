#include "w3ctestclienthandler.h"
#include <QDebug>
#include <QThread>

W3cTestClientHandler::W3cTestClientHandler(int nrOfClients, QList<W3cTestClient::TestCase> tests, bool randomize, bool secure, QString url )
{
    qRegisterMetaType<QList<W3cTestClient::TestCase>>();

    for(int i = 0; i < nrOfClients; i++)
    {
        QThread* clientThread = new QThread();

        W3cTestClient* client = new W3cTestClient(QUrl(url));
        client->moveToThread(clientThread);
        client->setTest(W3cTestClient::TestCase::AUTHORIZE_SUCCESS);

        connect(this,           &W3cTestClientHandler::startClients,    client,         &W3cTestClient::startClient);           // Start Client
        connect(client,         &W3cTestClient::completed,              this,           &W3cTestClientHandler::handleResult);   // Get Result from Client

        connect(client,         &W3cTestClient::completed,              client,         &QObject::deleteLater);                 // When done, delete Client
        connect(client,         &W3cTestClient::completed,              clientThread,   &QThread::quit);                        // When done, quit thread
        connect(clientThread,   &QThread::finished,                     clientThread,   &QObject::deleteLater);                 // When thread done, delete Thread

        qDebug() << "Starting client" <<  QThread::currentThreadId();

        clientThread->start();
        clients.append(clientThread);

        emit startClients(tests);
    }
    //emit startClients(test); // If we want all to run at the same time, could be toublesome if we got many clients.
}

W3cTestClientHandler::~W3cTestClientHandler()
{

}

void W3cTestClientHandler::handleResult(bool result)
{

}
