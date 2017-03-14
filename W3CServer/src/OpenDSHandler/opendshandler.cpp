#include "opendshandler.h"
#include <QPointer>
#include <QSettings>
#include <QDebug>
#include <QStringBuilder>
#include <QTcpSocket>
#include <QTime>
#include <QCoreApplication>
#include <QDomDocument>

OpenDSHandler::OpenDSHandler(QObject *parent) : QObject(parent)
{
    qDebug() << "handler executing";

    //Initiate socket and connect to simulation server (note that IP needs to be changed to match your server IP)
    m_Socket = new QTcpSocket(this);
    connect(m_Socket, &QTcpSocket::connected,this, &OpenDSHandler::connected);
    connect(m_Socket, &QTcpSocket::disconnected,this, &OpenDSHandler::disconnected);
    connect(m_Socket, &QTcpSocket::bytesWritten,this, &OpenDSHandler::bytesWritten);
    connect(m_Socket, &QTcpSocket::readyRead,this, &OpenDSHandler::readyRead);
    connect(m_Socket, static_cast<void(QAbstractSocket::*)(QAbstractSocket::SocketError)>(&QAbstractSocket::error), this, &OpenDSHandler::socketError);

    // Reading server settings from settings file
    QPointer<QSettings> settings = new QSettings();
    settings->beginGroup("OpenDSHandler");
    settings->beginGroup("servers");
    m_server_ip = settings->value("server_ip").toString();
    m_server_port = settings->value("server_port").toInt();
    m_delay_sec = settings->value("delay_sec").toInt();

    m_Socket->connectToHost(m_server_ip, m_server_port);
}

void OpenDSHandler::reconnect()
{
    //if connection to OpenDS Server was lost we wait for x seconds and then retries!
    delay(m_delay_sec);
    // "times up, should now retry....";
    m_Socket->connectToHost(m_server_ip, m_server_port);
}

void OpenDSHandler::delay(int delay)
{
    QTime delayTime = QTime::currentTime().addSecs(delay);
    while (QTime::currentTime() < delayTime)
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
}

void OpenDSHandler::xmlParser(QString xmlData)
{
    //Get your xml into xmlText(you can use QString instead og QByteArray)
    QDomDocument doc;
    doc.setContent(xmlData);

    //Parse data
    QDomNodeList speed=doc.elementsByTagName("speed");
    QDomNodeList rpm=doc.elementsByTagName("actualRpm");

    //notify listners for valueChanged
    emit valueChanged(VSSSignalInterfaceImpl::CarSignalType::Speed, speed.at(0).toElement().text());
    emit valueChanged(VSSSignalInterfaceImpl::CarSignalType::RPM, rpm.at(0).toElement().text());
}

void OpenDSHandler::connected()
{
    qDebug() << "connected to OpenDS Server, sending subscribe message";
    QByteArray message = getSubscribeMessage();
    qDebug() << message;

    m_Socket->write(message);
}

void OpenDSHandler::disconnected()
{
    qDebug() << "disconnected... reconnecting";
    reconnect();
}

void OpenDSHandler::bytesWritten(qint64 bytes)
{
    qDebug() << bytes << " bytes written...";
}

void OpenDSHandler::readyRead()
{
    // read the data from the socket
   xmlParser(m_Socket->readAll());
}

void OpenDSHandler::socketError(QAbstractSocket::SocketError error)
{
    qDebug() << "something went wrong...." << error;

    // check if socket is still connected or if we need to reconnect!
    if(!(m_Socket->state() == QTcpSocket::ConnectedState))
    {
        qDebug() << "Trying to recover";
        reconnect();
    }
}


QByteArray OpenDSHandler::getSubscribeMessage()
{
    // Reading the SubscribeMessage from settings file
    QPointer<QSettings> settings = new QSettings();
    settings->beginGroup("OpenDSHandler");
    int size = settings->beginReadArray("SubscribeMessage");

    // Constructing readSubscribeMessage from settings file
    QString message = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r";
    message = message % "<Message>""\r";
    for (int i = 0; i < size; ++i)
    {
        settings->setArrayIndex(i);
        QString key = settings->allKeys().first();
        QString value = settings->value(key).toString();
        QString event = "<Event Name=\"" % key % "\">" % value % "</Event>\r";
        message = message % event;
    }
    message = message % "</Message>\r";

    settings->endArray();
    settings->endGroup();

    return message.toLocal8Bit();
}

