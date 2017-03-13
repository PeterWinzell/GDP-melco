#include <QtCore/QCoreApplication>
#include "w3ctestclient.h"
#include <QDebug>


int main(int argc, char *argv[])
{
    qDebug() << "Client: main started";

    QCoreApplication a(argc, argv);

    // if only one argument, localhost is default and first argument is option
    QString host = "wss://127.0.0.1:8080";
    QString option = argv[1];

    // if 2 arguments, first is host, second is option
    if (argc > 2)
    {
        host = QString(argv[1]);
        option = argv[2];
    }

    QUrl url(host);
    W3cTestClient client(url);

    // Parse options
    if (argc > 1)
    {
        QString str(option);
        if (option == "-subscribe")
        {
            client.setTest(W3cTestClient::TestCase::SUBSCRIBE_UNSUBSCRIBE);
        }
        else if (option == "-subscribeall")
        {
            client.setTest(W3cTestClient::TestCase::SUBSCRIBEALL_UNSUBSCRIBEALL);
        }
        else if (option == "-authorize")
        {
            client.setTest(W3cTestClient::TestCase::AUTHORIZE_SUCCESS);
        }
        else if (option == "-getvss")
        {
            client.setTest(W3cTestClient::TestCase::GET_VSS);
        }
        else if (option == "-setget")
        {
            client.setTest(W3cTestClient::TestCase::SET_GET);
        }
        else
        {
            //Default
            qDebug() << "Unknown argument: " << str;
            qDebug() << "Running default test";
            client.setTest(W3cTestClient::TestCase::AUTHORIZE_SUCCESS);
            //client.setTest(W3cTestClient::TestCase::SUBSCRIBE_UNSUBSCRIBE);
        }
    }

    Q_UNUSED(client);

    return a.exec();
}
