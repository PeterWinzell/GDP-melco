#include "opendshandler.h"
#include <QPointer>
#include <QSettings>
#include <QDebug>
#include <QStringBuilder>
#include <QTcpSocket>

OpenDSHandler::OpenDSHandler(QObject *parent) : QObject(parent)
{
    // Reading server settings from settings file
    QPointer<QSettings> settings = new QSettings();
    settings->beginGroup("OpenDSHandler");
    settings->beginGroup("servers");
    QString server_ip = settings->value("server_ip").toString();
    int server_port = settings->value("server_port").toInt();

    //Initiate socket and connect to simulation server (note that IP needs to be changed to match your server IP)
    QTcpSocket socket = new QTcpSocket(this);
    connect(socket, SIGNAL(connected()),this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()),this, SLOT(disconnected()));
    connect(socket, SIGNAL(bytesWritten(qint64)),this, SLOT(bytesWritten(qint64)));
    connect(socket, SIGNAL(readyRead()),this, SLOT(readyRead()));




    socket->connectToHost(server_ip, server_port);

//    m_notifier(socket.);

//    connect(&_nam, &QNetworkAccessManager::networkAccessibleChanged, this, &SpaCloudPinger::onNetworkAccessibleChanged);

//    //Initiate socket and connect to simulation server (note that IP needs to be changed to match your server IP)
//    socket = new QTcpSocket(this);
//    socket->connectToHost("192.168.31.119", 5678);
//    //192.168.31.107


//    qDebug() << "Trying to connect...";
//    if(socket->waitForConnected(2000))
//    {
//        qDebug() << "Connected!";
//    }


//    QString message = readSubscribeMessage();
//    qDebug() << "SubscribeMessage\n" << message;

}


void MyTcpSocket::connected()
{
    qDebug() << "connected...";

    // Hey server, tell me about you.
    socket->write("HEAD / HTTP/1.0\r\n\r\n\r\n\r\n");
}

void MyTcpSocket::disconnected()
{
    qDebug() << "disconnected...";
}

void MyTcpSocket::bytesWritten(qint64 bytes)
{
    qDebug() << bytes << " bytes written...";
}

void MyTcpSocket::readyRead()
{
    qDebug() << "reading...";

    // read the data from the socket
    qDebug() << socket->readAll();
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
