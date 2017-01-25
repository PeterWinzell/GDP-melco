#ifndef W3CSERVER_H
#define W3CSERVER_H

#include <QObject>
#include <QList>
#include <QByteArray>

QT_FORWARD_DECLARE_CLASS(QWebSocketServer)
QT_FORWARD_DECLARE_CLASS(QWebSocket)

class W3CServer : public QObject
{
    Q_OBJECT
public:
    explicit W3CServer(quint16 port,bool debug=false, QObject *parent = Q_NULLPTR);
    ~W3CServer();

Q_SIGNALS:
    void closed();

private  Q_SLOTS:
    void onNewConnection();
    void processTextMessage(QString message);
    void socketDisconnected();

private:
  QWebSocketServer *m_pWebSocketServer;
  QList<QWebSocket *> m_clients;
  bool m_debug;
};

#endif // W3CSERVER_H
