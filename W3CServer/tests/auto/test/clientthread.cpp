#include "clientthread.h"

int ClientThread::noOfClientThreads = 0;

void ClientThread::run()
{
    qDebug() << "From client thread: " << currentThreadId();

    ClientThread::noOfClientThreads++;
    qDebug() << "No of client threads: " << ClientThread::noOfClientThreads;

    //Connect finished signal of the thread to lhandler
    connect(this, &QThread::finished, this, &ClientThread::onThreadFinished);

    client = new W3cTestClient(QUrl(QStringLiteral("wss://192.168.31.139:8080")));

    exec();
}

void ClientThread::onThreadFinished()
{
    qDebug() << "ClientThread::onThreadFinished(" << " From thread: " << QThread::currentThreadId();
    ClientThread::noOfClientThreads--;
    qDebug() << "No of client threads: " << ClientThread::noOfClientThreads;
    if (ClientThread::noOfClientThreads <= 0)
    {
        qDebug() << "All clients closed.";
        qApp->quit();
        //qApp->exit(0);

        qDebug() << "Quitting main";
    }
}


ClientThread::~ClientThread()
{
    if (client)
    {
        delete client;
    }
}
