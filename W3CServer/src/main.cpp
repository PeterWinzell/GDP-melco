#include <QCoreApplication>
#include <QtCore/QCommandLineParser>
#include <QtCore/QCommandLineOption>
#include "w3cserver.h"
#include <QSettings>
#include <QPointer>
#include <QFileInfo>
#include <QFile>

#include <OpenDSHandler/opendshandler.h>


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    //initiating QSettings
    QCoreApplication::setApplicationName("W3CServer");
    QCoreApplication::setOrganizationName("MelcoGOT");
    QSettings::setDefaultFormat(QSettings::IniFormat);
    QPointer<QSettings> settings = new QSettings();
    QSharedPointer<QFileInfo> checkFile(new QFileInfo(settings->fileName()));

    //check if settings file exists on local filesystem otherwise copy
    //the default settings file from resources
    if(!checkFile->isFile())
    {
        //could we copy?
        if (QFile::copy(":/W3CServer.ini",settings->fileName()))
        {
            //waits for settings file to become available and loads content
            while(!checkFile->isFile());
            settings->sync();
        }
    }

//qDebug() << "här är jag";
  //  OpenDSHandler handler;
//qDebug() << "nu är jag inte där...";

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
