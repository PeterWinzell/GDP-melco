#include <QtCore/QCoreApplication>
#include "w3ctestclient.h"
#include <QDebug>


int main(int argc, char *argv[])
{
    qDebug() << "Client: main started";

    QCoreApplication a(argc, argv);
    W3cTestClient client(QUrl(QStringLiteral("wss://192.168.31.116:8080")));

    Q_UNUSED(client);

    return a.exec();
}
