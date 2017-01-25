#include <QCoreApplication>
#include <QtCore/QCommandLineParser>
#include <QtCore/QCommandLineOption>
#include "w3cserver.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    /*QCommandLineParser parser;
    parser.setApplicationDescription("QtWebSockets example: W3cServer");
    parser.addHelpOption();

    QCommandLineOption dbgOption(QStringList() << "d" << "debug",
                                 QCoreApplication::translate("main", "Debug output [default: off]."));
    parser.addOption(dbgOption);

    QCommandLineOption portOption(QStringList() << "p" << "port",
                                  QCoreApplication::translate("main", "Port for w3cserver [default: 1234]."),
                                  QCoreApplication::translate("main", "port"), QLatin1Literal("1234"));

    parser.addOption(portOption);
    parser.process(a);
    bool debug = parser.isSet(dbgOption);
    int port = parser.value(portOption).toInt();*/

    W3CServer *server = new W3CServer(8080, true);
    QObject::connect(server, &W3CServer::closed, &a, &QCoreApplication::quit);

    return a.exec(); // start exec loop

}
