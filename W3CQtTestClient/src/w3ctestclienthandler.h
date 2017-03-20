#ifndef W3CTESTCLIENTHANDLER_H
#define W3CTESTCLIENTHANDLER_H

#include <QThread>
#include <QXmlStreamWriter>
#include <QFile>

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
    void writeXMLReport(QString filename);


   QVector<QThread*> m_clients;
   QList<ClientReport*> m_finishedClients;
   //int m_finishedClients = 0;
};

#endif // W3CTESTCLIENTHANDLER_H
