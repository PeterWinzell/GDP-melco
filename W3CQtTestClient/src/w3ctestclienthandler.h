#ifndef W3CTESTCLIENTHANDLER_H
#define W3CTESTCLIENTHANDLER_H

#include "w3ctestclient.h"

Q_DECLARE_METATYPE(QList<W3cTestClient::TestCase>)

class W3cTestClientHandler: public QObject
{
    Q_OBJECT
public:
    W3cTestClientHandler(int nrOfClients, QList<W3cTestClient::TestCase> tests, bool randomize, bool secure, QString url);
    ~W3cTestClientHandler();

public slots:
    void handleResult(bool result);

signals:
    void startClients(const QList<W3cTestClient::TestCase> &);

private:
   //QVector<QSharedPointer<W3cTestClient>> clients;
   QVector<QThread*> clients;
};

#endif // W3CTESTCLIENTHANDLER_H
