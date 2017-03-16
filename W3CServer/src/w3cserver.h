#ifndef W3CSERVER_H
#define W3CSERVER_H

#include <QObject>
#include <QList>
#include <QByteArray>
#include <QSslError>
#include <QMutex>
#include "OpenDSHandler/opendshandler.h"

class WebSocketWrapper;
class VSSSignalInterface;

//TODO add license header: Peter Winzell

QT_FORWARD_DECLARE_CLASS(QWebSocketServer)
QT_FORWARD_DECLARE_CLASS(QWebSocket)


class W3CServer : public QObject
{
    Q_OBJECT
public:
    explicit W3CServer(quint16 port,bool usesecureprotocol = false,bool debug=false, QObject *parent = Q_NULLPTR);
    ~W3CServer();

Q_SIGNALS:
    void closed();

private  Q_SLOTS:
    void onNewConnection();
    void processTextMessage(const QString& message);
    void socketDisconnected();
    void onSslErrors(const QList<QSslError> &errors);

private:
    void startRequestProcess(WebSocketWrapper* sw, const QString& message);
    QWebSocketServer *m_pWebSocketServer;
    QMap<QWebSocket *,QMutex*> m_clients;
    QSharedPointer<VSSSignalInterface> m_vsssInterface;
    QSharedPointer<OpenDSHandler> m_openDSHandler;
    bool m_debug;
    bool m_secure;
};

#endif // W3CSERVER_H
