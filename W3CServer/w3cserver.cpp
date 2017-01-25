#include "w3cserver.h"
#include "QtWebSockets/qwebsocketserver.h"
#include "QtWebSockets/qwebsocket.h"
#include <QDebug>

QT_USE_NAMESPACE

W3CServer::W3CServer(quint16 port, bool debug, QObject *parent) : QObject(parent),
    m_pWebSocketServer(new QWebSocketServer(QStringLiteral("W3CServer Test"),QWebSocketServer::NonSecureMode,this)),
    m_clients(),
    m_debug(debug)
{
    if (m_pWebSocketServer -> listen(QHostAddress::Any, port)){
        if (m_debug)
            qDebug() << "W3CServer is listening on port " << port;

        //Connect QWebSocketServer newConnection signal with W3cServer slot onNewConnection
        connect(m_pWebSocketServer, &QWebSocketServer::newConnection,this,&W3CServer::onNewConnection);
        //Connect QWebsocketServer signal with W3CServer signal closed
        connect(m_pWebSocketServer,&QWebSocketServer::closed, this, &W3CServer::closed);
    }

}

W3CServer::~W3CServer()
{
    m_pWebSocketServer -> close();
    //clean out all connected clients
    qDeleteAll(m_clients.begin(),m_clients.end());
}

void W3CServer::onNewConnection(){
    QWebSocket *pSocket = m_pWebSocketServer->nextPendingConnection();

    // Connect socket textMessageReceived signal with server processTextMessage slot
    connect(pSocket, &QWebSocket::textMessageReceived, this, &W3CServer::processTextMessage);
    // Connect socket disconnected signal with server socketDisconnected slot
    connect(pSocket, &QWebSocket::disconnected, this, &W3CServer::socketDisconnected);

    // add socket to list of clients
    m_clients << pSocket;
}


void W3CServer::processTextMessage(QString message){

    QWebSocket *zeClient = qobject_cast<QWebSocket *> (sender());

    if (m_debug)
        qDebug() << "Message recieved: " << message;
    if (zeClient){
        zeClient -> sendTextMessage(" Message returned from server to client " );

    }
}

void W3CServer::socketDisconnected()
{
     QWebSocket *zeClient = qobject_cast<QWebSocket *> (sender());
     if (m_debug)
         qDebug() << " socket disconnected: " << zeClient;

     //remove from client list and delete from heap
     if (zeClient){
         m_clients.removeAll(zeClient);
         zeClient -> deleteLater();
     }

}
