#include "w3cserver.h"
#include "QtWebSockets/qwebsocketserver.h"
#include "QtWebSockets/qwebsocket.h"
#include <QDebug>
#include <QFile>
#include <QSslCertificate>
#include <QSslKey>
#include<QThreadPool>
#include "jsonrequestparser.h"
#include<QJsonDocument>
#include<QJsonObject>
#include<QJsonArray>
#include "request-handler/processrequesttask.h"
#include "jwt-utility/qjsonwebtoken.h"
#include "jwt-utility/visstokenvalidator.h"

QT_USE_NAMESPACE

class W3CServer;

W3CServer::W3CServer(quint16 port,bool usesecureprotocol, bool debug, QObject *parent) : QObject(parent),
    m_pWebSocketServer(0),
    m_clients(),
    m_debug(debug),
    m_secure(usesecureprotocol)
{
    if (usesecureprotocol)
    {
        m_pWebSocketServer = new QWebSocketServer(QStringLiteral("W3CServer"),
                QWebSocketServer::SecureMode,
                this);
        // m_pWebSocketServer ->
        QSslConfiguration sslConfiguration;

        QFile keyFile(QStringLiteral(":/server.key"));
        QFile certFile(QStringLiteral(":/server.crt"));

        certFile.open(QIODevice::ReadOnly);
        const QByteArray bytes = certFile.readAll();

        qDebug() << "cert file length : " + QString::number(bytes.length());
        QSslCertificate certificate(bytes, QSsl::Pem);

        keyFile.open(QIODevice::ReadOnly);
        const QByteArray passphrase("6610");
        QSslKey sslKey(&keyFile, QSsl::Rsa, QSsl::Pem,QSsl::PrivateKey,passphrase);

        certFile.close();
        keyFile.close();

        sslConfiguration.setPeerVerifyMode(QSslSocket::VerifyNone);
        sslConfiguration.setLocalCertificate(certificate);
        sslConfiguration.setPrivateKey(sslKey);
        sslConfiguration.setProtocol(QSsl::TlsV1SslV3);

        m_pWebSocketServer->setSslConfiguration(sslConfiguration);
    }
    else
    {
        m_pWebSocketServer = new QWebSocketServer(QStringLiteral("W3CServer Test"),QWebSocketServer::NonSecureMode,this);
    }
    if (m_pWebSocketServer->listen(QHostAddress::Any, port))
    {
        if (m_debug)
        {
            qDebug() << "W3CServer is listening on port " << port;
        }

        //Connect QWebSocketServer newConnection signal with W3cServer slot onNewConnection
        connect(m_pWebSocketServer, &QWebSocketServer::newConnection,this,&W3CServer::onNewConnection);
        //Connect QWebsocketServer signal with W3CServer signal closed
       // connect(m_pWebSocketServer,&QWebSocketServer::closed, this, &W3CServer::closed);
        //SSL error handler
        connect(m_pWebSocketServer, &QWebSocketServer::sslErrors,
                this, &W3CServer::onSslErrors);
    }
}

W3CServer::~W3CServer()
{
    m_pWebSocketServer->close();
    //clean out all connected clients
    qDeleteAll(m_clients.begin(),m_clients.end());
}

void W3CServer::onNewConnection()
{
    QWebSocket *pSocket = m_pWebSocketServer->nextPendingConnection();
    // pSocket ->
    qDebug() << " attempting to connect ";

    // Connect socket textMessageReceived signal with server processTextMessage slot
    connect(pSocket, &QWebSocket::textMessageReceived, this, &W3CServer::processTextMessage);
    // Connect socket disconnected signal with server socketDisconnected slot
    connect(pSocket, &QWebSocket::disconnected, this, &W3CServer::socketDisconnected);

    // add socket to list of clients
    m_clients << pSocket;
}

void W3CServer::processTextMessage(QString message)
{

    QWebSocket *zeClient = qobject_cast<QWebSocket *> (sender());

    if (m_debug)
    {
        qDebug() << "Message recieved: " << message;
    }

    //testing
    /*QJsonDocument doc;
    doc = QJsonDocument::fromJson(message.toUtf8());

    qDebug() << " doc is " + doc.toJson();

    QJsonObject obj = doc.object();
    QString str = obj["action"].toString();
    qDebug() <<  " action is : " + str ;

    QString str2 = obj["requestId"].toString();
    qDebug() << " requestID is : " + str2;

    QJsonObject tokensObject = obj["tokens"].toObject();
    QString token = tokensObject["authorization"].toString();

    qDebug() << " token is : " + token;

    VissTokenValidator tokenValidator(token);
    if (tokenValidator.validateToken("mydirtysecret"))
        qDebug() << " TOKEN IS VERIFIED \n";
    else
        qDebug() << " TOKEN IS NOT VERIFIED \n";

    QString zePayload = tokenValidator.getJsonPayload();

    qDebug() << " token payload is " + zePayload;

    QJsonDocument doc2;
    doc2 = QJsonDocument::fromJson(zePayload.toUtf8());
        default:
            break;
    QJsonObject tokenpl = doc2.object();
    QString issuer = tokenpl["iss"].toString();
    qDebug() << " Token issuer is : " + issuer;
    QString valid_from = tokenpl["ValidFrom"].toString();
    qDebug() << " ValidFrom : " + valid_from;

    QString valid_to = tokenpl["ValidTo"].toString();
    qDebug() << " Valid To : " + valid_to;

    QString path = tokenpl["path"].toString();
    qDebug() << " Signal path is : " + path;

    QString actions = tokenpl["actions"].toString();
    qDebug() << " Actions are : " + actions;*/

    startRequestProcess(zeClient,message);
}

void W3CServer::socketDisconnected()
{
    QWebSocket *zeClient = qobject_cast<QWebSocket *> (sender());
    if (m_debug)
    {
        qDebug() << " socket disconnected: " << zeClient;
    }

    //remove from client list and delete from heap
    if (zeClient)
    {
        m_clients.removeAll(zeClient);
        zeClient->deleteLater();
    }
}

void W3CServer::onSslErrors(const QList<QSslError> &)
{
    qDebug() << "Ssl error occurred";
}


void W3CServer::startRequestProcess(QWebSocket* cl, QString message)
{
    ProcessRequestTask* requesttask = new ProcessRequestTask(cl, message, true);
    // QThreadPool takes ownership and deletes 'requesttask' automatically
    QThreadPool::globalInstance()->start(requesttask);
}
