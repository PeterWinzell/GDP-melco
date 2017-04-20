#ifndef SignalServer_H
#define SignalServer_H

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

class SignalServer : public QObject
{
    Q_OBJECT
public:
    explicit SignalServer(quint16 port, QObject *parent = Q_NULLPTR);
    ~SignalServer();

    static int m_nrOfClients;
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
    bool m_secure;

};

#endif // SignalServer_H
