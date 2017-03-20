#include "w3ctestclienthandler.h"
#include <QDebug>
#include <QtCore/QCoreApplication>


W3cTestClientHandler::W3cTestClientHandler(int nrOfClients, QQueue<TestCase> tests, bool randomize, bool secure, QString url )
{
    Q_UNUSED(randomize);
    Q_UNUSED(secure);

    qRegisterMetaType<QQueue<TestCase>>();

    for(int i = 0; i < nrOfClients; i++)
    {
        QThread* clientThread = new QThread();

        W3cTestClient* client = new W3cTestClient(i+1, tests, QUrl(url));
        client->moveToThread(clientThread);

        connect(this,           &W3cTestClientHandler::startClients,
                client,         &W3cTestClient::startClient);           // Start Client

        connect(client,         &W3cTestClient::testsfinished,
                this,           &W3cTestClientHandler::handleTestClientCompletion);   // When all Tests are done on Client

        connect(client,         &W3cTestClient::testsfinished,
                client,         &QObject::deleteLater);                 // When done, delete Client

        connect(client,         &W3cTestClient::testsfinished,
                clientThread,   &QThread::quit);                        // When done, quit thread

        connect(clientThread,   &QThread::finished,
                clientThread,   &QObject::deleteLater);                 // When thread done, delete Thread

        clientThread->start();
        m_clients.append(clientThread);

        emit startClients();
    }
    //emit startClients(test); // If we want all to run at the same time, could be toublesome if we got many clients.
}

W3cTestClientHandler::~W3cTestClientHandler()
{
    qDebug() << " Handler Deleted!";

}

void W3cTestClientHandler::handleTestClientCompletion(ClientReport* report)
{
    qDebug() << " Test Client Finished!";
    m_finishedClients.append(report);

    if(m_finishedClients.length() >= m_clients.length())
    {
        QString file = QString("/home/vagrant/GDP/GDP-melco/tests.xml");
        writeXMLReport(file);
        QCoreApplication::exit(0);
    }
}
void W3cTestClientHandler::writeXMLReport(QString filename)
{
    QFile file(filename);
    if(!file.open(QFile::WriteOnly |QFile::Text))
    {
        return;
    }

    QXmlStreamWriter stream(&file);
    stream.setAutoFormatting(true);
    stream.writeStartDocument();

    stream.writeStartElement("system-test");
    stream.writeAttribute("sw-version", "NA");
    stream.writeAttribute("randomized", "NA");
    stream.writeAttribute("timestamp", "NA");

    for(auto report : m_finishedClients)
    {
        stream.writeStartElement("test-client");
        stream.writeAttribute("id", QString::number(report->m_clientId));
        for (auto results : report->m_testResults)
        {
            stream.writeStartElement("test");
            stream.writeAttribute("id", "NA");
            for (auto result : results.keys())
            {
                stream.writeTextElement(result, results.value(result));
            }
            stream.writeEndElement();
        }
         stream.writeEndElement();
    }

    stream.writeEndElement();

    stream.writeEndDocument();

    file.flush();
    file.close();
}
