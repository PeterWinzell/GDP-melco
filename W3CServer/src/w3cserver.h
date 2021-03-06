#ifndef W3CSERVER_H
#define W3CSERVER_H

#include <QObject>
#include <QList>
#include <QByteArray>
#include <QSslError>
#include <QMutex>

class WebSocketWrapper;
class VSSSignalInterface;

//TODO add license header: Peter Winzell

QT_FORWARD_DECLARE_CLASS(QWebSocketServer)
QT_FORWARD_DECLARE_CLASS(QWebSocket)

class W3CServer : public QObject
{
    Q_OBJECT
public:
    explicit W3CServer(quint16 port,bool usesecureprotocol = false, QObject *parent = Q_NULLPTR);
    ~W3CServer();

    static int m_nrOfClients;
Q_SIGNALS:
    void closed();

private  Q_SLOTS:
    void onNewConnection();
    void processTextMessage(const QString& message);
    void socketDisconnected();
    void onSslErrors(const QList<QSslError> &errors);
    void closingDown();

    void insertDefaultAuthObject(QWebSocket* aSocket);
private:
    void startRequestProcess(WebSocketWrapper* sw, const QString& message);
    QWebSocketServer *m_pWebSocketServer;
    QMap<QWebSocket *,QMutex*> m_clients;
    QSharedPointer<VSSSignalInterface> m_vsssInterface;
    bool m_secure;
    bool m_authmanagement_off;

};

#endif // W3CSERVER_H
