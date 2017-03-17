#ifndef CLIENTREPORT_H
#define CLIENTREPORT_H

class ClientReport
{
public:
    ClientReport(int clientId) { m_clientId = clientId; }

    int m_clientId;
    QList<QHash<QString,QString>> m_testResults;

};

#endif // CLIENTREPORT_H
