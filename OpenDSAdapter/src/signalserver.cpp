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


QT_USE_NAMESPACE

class SignalServer;
int SignalServer::m_nrOfClients;
SignalServer::SignalServer(quint16 port, QObject *parent) : QObject(parent),
    m_pWebSocketServer(0),
    m_clients()
{
    // Set up connection to Open DS
    m_openDSHandler = QSharedPointer<OpenDSHandler>(new OpenDSHandler());
    QObject::connect(m_openDSHandler.data(), &OpenDSHandler::valueChanged, m_openDSHandler.data(), &OpenDSHandler::updateValue);

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
}

void SignalServer::onNewConnection()
{
    QWebSocket *pSocket = m_pWebSocketServer->nextPendingConnection();

    pSocket ->ignoreSslErrors();

    qDebug() << "Server: Attemping to connect";

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
    qDebug() << "Server Message received";

    QWebSocket *zeClient = qobject_cast<QWebSocket *> (sender());

    if (m_clients.contains(zeClient))
    {
        //Parse the received message. Example Get message:
        //
        //    "get": [ {"Signal.Cabin.Door.Row1.Right.IsLocked" : true },
        //             {"Signal.Cabin.Door.Row1.Left.IsLocked" : true },
        //             {"Signal.Cabin.Door.Row2.Right.IsLocked" : false },
        //             {"Signal.Cabin.Door.Row2.Left.IsLocked" : true } ],
        QJsonParseError parseError;
        QJsonDocument jsonRequest = QJsonDocument::fromJson(message.toUtf8(), &parseError);
        QJsonObject jsonMsg = jsonRequest.object();

        QJsonArray valueList;

        QJsonObject responseMsg;

        bool result = false;

        //First check whether Get or Set
        if (jsonMsg["get"].isArray())
        {
            QJsonArray responseValueList;

            valueList = jsonMsg["get"].toArray();

            // For each signal in list, get value from OpenDS
            foreach (QJsonValue entry, valueList)
            {
                QString signal = entry.toObject().keys().first();
                QVariant value = m_openDSHandler->getSignalValue(signal);

                qDebug() << "GET signal :" << signal << "value :" << value;

                // Store value in response Json
                QJsonObject responseValue;
                responseValue.insert(signal, QJsonValue::fromVariant(value));
                responseValueList.append(responseValue);
            }
            responseMsg.insert("get", responseValueList);
            result = true;
        }
        else if (jsonMsg["set"].isArray())
        {
            valueList = jsonMsg["set"].toArray();

            // For each signal in list, set value in OpenDS
            foreach (QJsonValue entry, valueList)
            {
                QString signal = entry.toObject().keys().first();
                QVariant value = entry.toObject().value(signal).toVariant();

                if (m_openDSHandler->setSignalValue(signal, value))
                {
                    qDebug() << "SET signal :" << signal << "value :" << value;
                    result = true;
                }
            }
            m_openDSHandler->updateSetSignalValues();

            responseMsg.insert("set", result);
        }
        else
        {
            qDebug() << "Json parse error: no set or get found";
        }

        QJsonDocument jsonDoc(responseMsg);
        QString responseMessage = jsonDoc.toJson();
        zeClient->sendTextMessage(responseMessage);

    }
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
    qDebug() << "Server SSL Error occurred: " << l;
}

