#include "opendshandler.h"
#include <QPointer>
#include <QSettings>
#include <QDebug>
#include <QStringBuilder>

OpenDSHandler::OpenDSHandler(QObject *parent) : QObject(parent)
{
    readSubscribeMessage();
}

QString OpenDSHandler::readSubscribeMessage()
{
    QPointer<QSettings> settings = new QSettings();

    // Reading the SubscribeMessage from settings file
    settings->beginGroup("OpenDSHandler");
    int size = settings->beginReadArray("SubscribeMessage");
    QString message = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r<Message>""\r";
    QStringList list;

    for (int i = 0; i < size; ++i)
    {
        settings->setArrayIndex(i);
        QString key = settings->allKeys().first();
        QString value = settings->value(key).toString();
        QString event = "<Event Name=\"" % key % "\">" % value % "</Event>\r";
        message = message % event;
    }
    message = message % "<Event Name=\"EstablishConnection\"/>\r";
    message = message % "</Message>";

    settings->endArray();
    settings->endGroup();

    qDebug() << "SubscribeMessage\n" << message;

    return message;


//    <?xml version="1.0" encoding="UTF-8"?>
//    <Message>
//    <Event Name="Unsubscribe">/root/thisVehicle</Event>
//    <Event Name="Subscribe">/root/thisVehicle/exterior/engineCompartment/engine/Properties/actualRpm</Event>
//    <Event Name="Subscribe">/root/thisVehicle/physicalAttributes/Properties/speed</Event>
//    <Event Name="SetUpdateInterval">200</Event>
//    <Event Name="EstablishConnection"/>
//    </Message>


//    //xml message for starting subscribing to RPM and speed
//    char xmlMsgSubscribe[]= "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\
//                  <Message>\r\
//                    <Event Name=\"Unsubscribe\">/root/thisVehicle</Event>\r\
//                    <Event Name=\"Subscribe\">/root/thisVehicle/exterior/engineCompartment/engine/Properties/actualRpm</Event>\r\
//                    <Event Name=\"Subscribe\">/root/thisVehicle/physicalAttributes/Properties/speed</Event>\r\
//                    <Event Name=\"SetUpdateInterval\">200</Event>\r \
//                    <Event Name=\"EstablishConnection\"/>\r \
//                  </Message>\r";
}
