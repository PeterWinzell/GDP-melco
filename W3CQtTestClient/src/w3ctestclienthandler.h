#ifndef W3CTESTCLIENTHANDLER_H
#define W3CTESTCLIENTHANDLER_H

#include "w3ctestclient.h"
#include "clientreport.h"

Q_DECLARE_METATYPE(QQueue<TestCase>)

class W3cTestClientHandler: public QObject
{
    Q_OBJECT
public:
    W3cTestClientHandler(int nrOfClients, QQueue<TestCase> tests, bool randomize, bool secure, QString url);
    ~W3cTestClientHandler();

public slots:
    void handleTestClientCompletion(ClientReport* report);

signals:
    void startClients();

private:
   //QVector<QSharedPointer<W3cTestClient>> clients;
   QVector<QThread*> m_clients;
   int m_finishedClients = 0;
};

#endif // W3CTESTCLIENTHANDLER_H
