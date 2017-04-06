#include <QCoreApplication>
#include <QtCore/QCommandLineParser>
#include <QtCore/QCommandLineOption>
#include "w3cserver.h"
#include <QSettings>
#include <QPointer>
#include <QFileInfo>
#include <QFile>
#include <QDir>

#include <OpenDSHandler/opendshandler.h>
#include <logger.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    // TODO Add as arguments
    Logger::logEnabled = true;
    Logger::logLevel = 1;
    Logger::logToFile = true;
    Logger::logFilename = "w3c-server-log.log";

    //initiating QSettings
    QCoreApplication::setApplicationName("W3CServer");
    QCoreApplication::setOrganizationName("MelcoGOT");
    QSettings::setDefaultFormat(QSettings::IniFormat);
    QPointer<QSettings> settings = new QSettings();
    QSharedPointer<QFileInfo> checkFile(new QFileInfo(settings->fileName()));

    INFO("Server", "Trying to open settings file: " + settings->fileName());

    //check if settings file exists on local filesystem otherwise copy
    //the default settings file from resources
    if(!checkFile->isFile())
    {
        INFO("Server","No settings file found, trying to copy defualt settings file...");

        if(QDir().mkpath(checkFile->absolutePath()))
        {
            TRACE("Server","Settings file path exists.");
            //could we copy?
            if (QFile::copy(":/W3CServer.ini",settings->fileName()))
            {
                TRACE("Server","Default settings file copied.");
                //waits for settings file to become available and loads content
                while(!checkFile->isFile());
                settings->sync();
            }
        }
    }

    // reads W3CServer settings values
    settings->beginGroup("W3CServer");
    qint16 serverPort = settings->value("server_port").toInt();
    bool serverWSS = settings->value("server_wss").toBool();
    bool serverDebug = settings->value("server_debug").toBool();
    settings->endGroup();

    Logger::logEnabled = serverDebug ? true : false;

    W3CServer *server = new W3CServer(serverPort,serverWSS);
    QObject::connect(server, &W3CServer::closed, &a, &QCoreApplication::quit);

    return a.exec(); // start exec loop
}
