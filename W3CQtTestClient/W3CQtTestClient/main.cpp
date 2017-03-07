#include <QtCore/QCoreApplication>
#include "w3ctestclient.h"
#include <QDebug>


int main(int argc, char *argv[])
{
    qDebug() << "Client: main started";

    QCoreApplication a(argc, argv);

    W3cTestClient client(QUrl(QStringLiteral("wss://127.0.0.1:8080")));

    // Parse options
    if (argc > 0)
    {
        QString str(argv[1]);
        if (str == "-subscribe")
        {
            client.setTest(W3cTestClient::TestCase::SUBSCRIBE_UNSUBSCRIBE);
        }
        else if (str == "-subscribeall")
        {
            client.setTest(W3cTestClient::TestCase::SUBSCRIBEALL_UNSUBSCRIBEALL);
        }
        else if (str == "-authorize")
        {
            client.setTest(W3cTestClient::TestCase::AUTHORIZE_SUCCESS);
        }
        else if (str == "-getvss")
        {
            client.setTest(W3cTestClient::TestCase::GET_VSS);
        }
        else if (str == "-setget")
        {
            client.setTest(W3cTestClient::TestCase::SET_GET);
        }
        else
        {
            //Default
            qDebug() << "Unknown argument: " << str;
            qDebug() << "Running default test";

            client.setTest(W3cTestClient::TestCase::SUBSCRIBE_UNSUBSCRIBE);
        }
    }

    Q_UNUSED(client);

    return a.exec();
}
