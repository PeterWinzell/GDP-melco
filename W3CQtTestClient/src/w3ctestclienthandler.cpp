#include "w3ctestclienthandler.h"
#include <QDebug>
#include <QThread>
#include <QtCore/QCoreApplication>
#include <QFileInfo>
#include "testcasedescriptions.h"

W3cTestClientHandler::W3cTestClientHandler(int nrOfClients, QQueue<TestCase> tests, QString url,
        QString swversion, QString timestamp, bool randomize, const QString& reportDir)
    : m_testCases(tests), m_url(url), m_swversion(swversion), m_timestamp(timestamp), m_randomize(randomize), m_reportDir(reportDir)
{
    qRegisterMetaType<QQueue<TestCase>>();

    for(int i = 0; i < nrOfClients; i++)
    {
        QThread* clientThread = new QThread();

        W3cTestClient* client = new W3cTestClient(i+1, tests, randomize, QUrl(url));
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
    qDebug() << "[Client#" << report->m_clientId << "] " << " Test Client Finished!";
    m_finishedClients.append(report);

    if(m_finishedClients.length() >= m_clients.length())
    {
        QString file = QString("./w3c-tests.xml");
        QString htmlFile = QString("./w3c-tests.html");

        if (m_reportDir != "")
        {
            htmlFile = m_reportDir + "/w3c-tests.html";
            file = m_reportDir + "/w3c-tests.xml";
        }
        qDebug() << "Generating reports...";
        writeXMLReport(file);
        writeHTMLReport(htmlFile);
        QCoreApplication::exit(0);
    }
}
void W3cTestClientHandler::writeXMLReport(QString filename)
{
    QFile file(filename);
    QFileInfo fileinfo(filename);
    if(!file.open(QFile::WriteOnly |QFile::Text))
    {
        qWarning() << "Failed to open file " << fileinfo.absoluteFilePath() << " for writing";
        return;
    }
    TestCaseDescriptions desc;

    QXmlStreamWriter stream(&file);
    stream.setAutoFormatting(true);
    stream.writeStartDocument();

    stream.writeStartElement("system-test");
    stream.writeAttribute("sw-version", m_swversion);
    stream.writeAttribute("timestamp", m_timestamp);
    stream.writeAttribute("url", m_url);
    stream.writeAttribute("randomized", m_randomize ? "true" : "false");

    // Add meta-data

    for(int i = 0; i < (int)TestCase::NR_OF_TESTCASES; i++)
    {
        stream.writeStartElement("system-test");
        stream.writeAttribute("id", QString::number(desc.getDescription(i)->m_id));
        stream.writeAttribute("name", desc.getDescription(i)->m_name);
        stream.writeAttribute("description", desc.getDescription(i)->m_description);
        stream.writeEndElement();
    }

    for(auto report : m_finishedClients)
    {
        stream.writeStartElement("test-client");
        stream.writeAttribute("id", QString::number(report->m_clientId));
        for (auto results : report->m_testResults)
        {
            stream.writeStartElement("test");
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

    qDebug() << "Xml report saved at " << fileinfo.absoluteFilePath();
}

void W3cTestClientHandler::writeHTMLReport(const QString& filename)
{
    QFile file(filename);
    QFileInfo fileinfo(filename);
    if(!file.open(QFile::WriteOnly |QFile::Text))
    {
        qWarning() << "Failed to open file " << fileinfo.absoluteFilePath() << " for writing";
        return;
    }

    TestCaseDescriptions desc;

    QString startDoc = "<!DOCTYPE html> <html>";
    QString endDoc = "</html>";

    QString startTable = "<table border=\"1\">";
    QString endTable = "</table>";
    QString startRow = "<tr>";
    QString endRow = "</tr>";
    QString startCol = "<td>";
    QString endCol = "</td>";

    QString startColGreen = "<td bgcolor=\"#32cb00\">";
    QString startColRed = "<td bgcolor=\"#fe0000\">";

    QString startHeader = "<th>";
    QString endHeader = "</th>";

    QString table = startTable;

    // create header

    table += startRow;

    table += startHeader;
    table += "Client id";
    table += endHeader;

    table += startHeader;
    table += "Test case";
    table += endHeader;

    table += startHeader;
    table += "Started";
    table += endHeader;

    table += startHeader;
    table += "Ended";
    table += endHeader;

    table += startHeader;
    table += "Outcome";
    table += endHeader;

    table += endRow;

    // and rest of the table

    for(auto report : m_finishedClients)
    {
        for (auto results : report->m_testResults)
        {
            table += startRow;

            table += startCol;
            table += QString::number(report->m_clientId);
            table += endCol;

            table += startCol;
            int caseId = results["testcase"].toInt();
            table += desc.getDescription(caseId)->m_name;
            table += endCol;

            table += startCol;
            table += results["started"];
            table += endCol;

            table += startCol;
            table += results["ended"];
            table += endCol;

            QString outcome = results["outcome"];
            if (outcome == "passed")
            {
                table += startColGreen;
            }
            else
            {
                outcome += startColRed;
            }

            table += outcome;
            table += endCol;

            table += endRow;
        }
    }

    table += endTable;

    QString html = startDoc + table + endDoc;

    QTextStream stream( &file );

    stream << html;

    file.close();

    qDebug() << "Html report saved at " << fileinfo.absoluteFilePath();
}
