#include <QtCore/QCoreApplication>
#include "w3ctestclient.h"
#include "w3ctestclienthandler.h"

#include <QDebug>
#include <QCommandLineParser>

int main(int argc, char *argv[])
{
    qDebug() << "Client: main started";
    QCoreApplication a(argc, argv);

    QCommandLineParser parser;
    parser.setApplicationDescription("W3C VISS Reference Implementation Test Client Application");
    parser.addHelpOption();
    //parser.addVersionOption();

    parser.addPositionalArgument("tests", QCoreApplication::translate("main", "Test case sto run.")); // TODO Add list to -help

    QCommandLineOption randomizeOption(QStringList() << "r" << "random", QCoreApplication::translate("main", "Randomize test case execution order."));
    parser.addOption(randomizeOption);

    QCommandLineOption secureOption(QStringList() << "s" << "secure", QCoreApplication::translate("main", "Use Secure Web Sockets."));
    parser.addOption(secureOption);

    QCommandLineOption urlOption(QStringList() << "u" << "url", QCoreApplication::translate("main", "Target W3CServer implementation url."));
    parser.addOption(urlOption);

    QCommandLineOption clientOption(QStringList() << "c" << "clients", QCoreApplication::translate("main", "Number of clients to use."));
    parser.addOption(clientOption);


    parser.process(a);

    int nrOfClients = 5; // Default number of clients
    if(parser.isSet(clientOption))
    {
        nrOfClients = parser.value(clientOption).toInt();    // Needs better validation
    }

    QList<W3cTestClient::TestCase> tests;
    for(auto test : parser.positionalArguments())
    {
        if (test == "subscribe")
        {
            tests << W3cTestClient::TestCase::SUBSCRIBE_UNSUBSCRIBE;
        }
        else if (test == "subscribeall")
        {
            tests << W3cTestClient::TestCase::SUBSCRIBEALL_UNSUBSCRIBEALL;
        }
        else if (test == "authorize")
        {
            tests << W3cTestClient::TestCase::AUTHORIZE_SUCCESS;
        }
        else if (test == "getvss")
        {
            tests << W3cTestClient::TestCase::GET_VSS;
        }
        else if (test == "setget")
        {
            tests << W3cTestClient::TestCase::SET_GET;
        }
        else
        {
            qDebug() << "Unknown argument: " << test;
        }
    }

    bool randomize = parser.isSet(randomizeOption);
    bool secure = parser.isSet(secureOption);


    QString url = "ws://127.0.0.1:8080"; // default url

    // Is url set, change url. If not, and secure is set, set to secure url, else use default url.
    if(parser.isSet(urlOption))
    {
        url = parser.value(urlOption);
    }
    else if(secure)
    {
        url = "wss://127.0.0.1:8080"; // default secure url
    }

    W3cTestClientHandler handler(nrOfClients, tests, randomize, secure, url);


    Q_UNUSED(handler);

    return a.exec();
}
