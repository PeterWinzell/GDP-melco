#include <QCoreApplication>
#include <QtCore/QCommandLineParser>
#include <QtCore/QCommandLineOption>
#include "w3cserver.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // check if we are running the server in secure mode (wss)
    bool sec = false;
    if (argc > 0)
    {
        QString str(argv[1]);
        if (str == "-secure")
            sec = true;
    }

    W3CServer *server = new W3CServer(8080,sec,true);
    QObject::connect(server, &W3CServer::closed, &a, &QCoreApplication::quit);

    return a.exec(); // start exec loop
}
