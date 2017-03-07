#include <QtCore/QCoreApplication>
#include "w3ctestclient.h"
#include <QDebug>


int main(int argc, char *argv[])
{
    qDebug() << "Client: main started";

    QCoreApplication a(argc, argv);
    W3cTestClient client(QUrl(QStringLiteral("wss://127.0.0.1:8080")));

    Q_UNUSED(client);

    return a.exec();
}
