#include <QCoreApplication>
#include <QtCore/QCommandLineParser>
#include <QtCore/QCommandLineOption>
#include "w3cserver.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QCoreApplication::setApplicationName("W3CServer");
    QCoreApplication::setOrganizationName("MelcoGOT");

    /* ToDo exchange QStandardPaths::HomeLocation to QStandardPaths::AppConfigLocation in order to save the INI file on a writable area */
    m_sSettingsFile = QStandardPaths::writableLocation(QStandardPaths::HomeLocation)+"/w3cserver.ini";

    //check if path is empty
    if (m_sSettingsFile.isEmpty())
    {
        qFatal("Cannot determine settings storage location");
    }

    //check if the settings file exists
    std::auto_ptr<QFileInfo> checkFile(new QFileInfo(m_sSettingsFile));
    if(!checkFile->exists())
    {
       createDefaultSettings();
    }


    // check if we are running the server in secure mode (wss)
    bool sec = false;
    if (argc > 0)
    {
        QString str(argv[1]);
        if (str == "-secure")
        {
            sec = true;
        }
    }



    W3CServer *server = new W3CServer(8080,sec,true);
    QObject::connect(server, &W3CServer::closed, &a, &QCoreApplication::quit);

    return a.exec(); // start exec loop
}
