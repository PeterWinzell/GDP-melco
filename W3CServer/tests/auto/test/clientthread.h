#ifndef CLIENTTHREAD_H
#define CLIENTTHREAD_H

#include <QtCore/QCoreApplication>
#include <QtCore>
#include <QThread>
#include "../../../../W3CQtTestClient/W3CQtTestClient/w3ctestclient.h"

class ClientThread : public QThread
{
    Q_OBJECT

public:
    ~ClientThread();
    static int noOfClientThreads;

protected:
    void run();

private:
    W3cTestClient *client;


private Q_SLOTS:
    void onThreadFinished();

};

#endif // CLIENTTHREAD_H
