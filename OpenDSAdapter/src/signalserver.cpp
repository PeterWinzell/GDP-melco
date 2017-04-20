#include <QWebSocketServer>
#include <QWebSocket>
#include <QDebug>
#include <QFile>
#include <QSslCertificate>
#include <QSslKey>
#include <QThreadPool>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QPointer>

#include "signalserver.h"
#include "VSSSignalinterface/vsssignalinterfaceimpl.h"
#include "VSSSignalinterface/vsssignalinterface.h"
//#include "opendshandler.h"

QT_USE_NAMESPACE

class SignalServer;
int SignalServer::m_nrOfClients;
SignalServer::SignalServer(quint16 port, QObject *parent) : QObject(parent),
    m_pWebSocketServer(0),
    m_clients()
{
    QThreadPool::globalInstance()->setMaxThreadCount(100);

    m_pWebSocketServer = new QWebSocketServer(QStringLiteral("SignalServer Test"),QWebSocketServer::NonSecureMode,this);

    if (m_pWebSocketServer->listen(QHostAddress::Any, port))
    {
        qDebug() << "SignalServer is listening on port " << QString::number(port);

        //Connect QWebSocketServer newConnection signal with SignalServer slot onNewConnection
        connect(m_pWebSocketServer, &QWebSocketServer::newConnection,this,&SignalServer::onNewConnection);
        //Connect QWebsocketServer signal with SignalServer signal closed
        // connect(m_pWebSocketServer,&QWebSocketServer::closed, this, &SignalServer::closed);
        //SSL error handler
        connect(m_pWebSocketServer, &QWebSocketServer::sslErrors,
                this, &SignalServer::onSslErrors);
    }
    else
    {
        qDebug() << "SignalServer error: failed to start on port " << QString::number(port);
    }
}

SignalServer::~SignalServer()
{
    m_pWebSocketServer->close();
    //clean out all connected clients
    qDeleteAll(m_clients.begin(),m_clients.end());
    m_vsssInterface.clear();
}

void SignalServer::onNewConnection()
{
    QWebSocket *pSocket = m_pWebSocketServer->nextPendingConnection();

    pSocket ->ignoreSslErrors();

    qDebug() << "Server","Attemping to connect";

    // Connect socket textMessageReceived signal with server processTextMessage slot
    connect(pSocket, &QWebSocket::textMessageReceived, this, &SignalServer::processTextMessage);
    // Connect socket disconnected signal with server socketDisconnected slot
    connect(pSocket, &QWebSocket::disconnected, this, &SignalServer::socketDisconnected);

    // add socket to list of clients
    m_clients.insert(pSocket, new QMutex());
    SignalServer::m_nrOfClients++;
}

void SignalServer::processTextMessage(const QString& message)
{
    qDebug() << "Server Message received" << message;

    //Parse the received message. Example Get message:
    //
    //    "get": [ {"Signal.Cabin.Door.Row1.Right.IsLocked" : true },
    //             {"Signal.Cabin.Door.Row1.Left.IsLocked" : true },
    //             {"Signal.Cabin.Door.Row2.Right.IsLocked" : false },
    //             {"Signal.Cabin.Door.Row2.Left.IsLocked" : true } ],
    QJsonParseError parseError;
    QJsonDocument jsonRequest = QJsonDocument::fromJson(message.toUtf8(), &parseError);
    QJsonObject jsonMsg = jsonRequest.object();

    //First check whether Get or Set
    QJsonArray valueList;

    if (jsonMsg["get"].isArray())
    {
        valueList = jsonMsg["get"].toArray();
    }
    else if (jsonMsg["set"].isArray())
    {
        valueList = jsonMsg["set"].toArray();
    }
    else
    {
        qDebug() << "Json parse error: no set or get found";
    }

    if (valueList.isEmpty())
    {
        qDebug() << "Json parse error: list is empty";
    }
    else
    {


    }



/*
    QString val = valueAction.toString();

    if (val == "get")
    {
        request->setAction(GET);



    QWebSocket *zeClient = qobject_cast<QWebSocket *> (sender());
    if (m_clients.contains(zeClient))
    {
        // we need a mutex per client .
        QMutex* mutex = m_clients.find(zeClient).value();
        QPointer<WebSocketWrapper> socketWrapper = new WebSocketWrapper(zeClient, mutex);
        startRequestProcess(socketWrapper, message);
    }
    else
    {
        WARNING("Server","Fatal connection error, Websocket client not found.");
    }
    */
}

void SignalServer::socketDisconnected()
{
    QWebSocket *zeClient = qobject_cast<QWebSocket *> (sender());

    qDebug() << "Server Socket disconnected";

    //remove from client list and delete from heap
    if (zeClient)
    {
        m_clients.remove(zeClient);
        zeClient->deleteLater();
        SignalServer::m_nrOfClients--;
    }
}

void SignalServer::onSslErrors(const QList<QSslError> &l)
{
    qDebug() << "Server SSL Error occurred.";
}

void SignalServer::startRequestProcess(WebSocketWrapper* sw, const QString& message)
{
}
