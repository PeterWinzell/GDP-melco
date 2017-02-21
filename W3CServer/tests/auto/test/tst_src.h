#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include "../../../../W3CQtTestClient/W3CQtTestClient/w3ctestclient.h"
#include <QtCore/QCoreApplication>
#include <QtCore>
#include <QThread>
#include "clientthread.h"
using namespace testing;

TEST(W3CServerClass, multipleClients)
{
    int argc = 0;
    char *argv[1];
    const int noOfClients = 4;
    std::vector<ClientThread> clients(noOfClients);

    QCoreApplication a(argc, argv);

    qDebug() << "From main thread: " << QThread::currentThreadId();

    for (int i=0; i<noOfClients; i++)
    {
        clients[i].start();
    }

    ASSERT_THAT(0, a.exec());
}
