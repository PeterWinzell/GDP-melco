#ifndef SignalServer_H
#define SignalServer_H

#include <QObject>
#include <QList>
#include <QByteArray>
#include <QSslError>
#include <QMutex>
#include "OpenDSHandler/opendshandler.h"


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
    QWebSocketServer *m_pWebSocketServer;
    QMap<QWebSocket *,QMutex*> m_clients;
    bool m_secure;
    QSharedPointer<OpenDSHandler>m_openDSHandler;

};

#endif // SignalServer_H
