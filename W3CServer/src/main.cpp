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
    qInstallMessageHandler(Logger::messageHandler);



    //initiating QSettings
    QCoreApplication::setApplicationName("W3CServer");
    QCoreApplication::setOrganizationName("MelcoGOT");
    QSettings::setDefaultFormat(QSettings::IniFormat);
    QPointer<QSettings> settings = new QSettings();
    QSharedPointer<QFileInfo> checkFile(new QFileInfo(settings->fileName()));

    qDebug() << "Try to open settings file: " << settings->fileName();

    //check if settings file exists on local filesystem otherwise copy
    //the default settings file from resources
    if(!checkFile->isFile())
    {
        qDebug() << "No settings file found";

        if(QDir().mkpath(checkFile->absolutePath()))
        {
            qDebug() << "Path exists";

            //could we copy?
            if (QFile::copy(":/W3CServer.ini",settings->fileName()))
            {
                qDebug() << "Default settings file copied";

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

    W3CServer *server = new W3CServer(serverPort,serverWSS,serverDebug);
    QObject::connect(server, &W3CServer::closed, &a, &QCoreApplication::quit);

    return a.exec(); // start exec loop
}
