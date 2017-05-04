#ifndef WEBSOCKETWRAPPER_H
#define WEBSOCKETWRAPPER_H
#include <QMutex>
#include <QObject>
#include <QWebSocket>

class WebSocketWrapper : public QObject
{
    Q_OBJECT
public:
    explicit WebSocketWrapper(QWebSocket* socket, QMutex* mutex, QObject *parent = Q_NULLPTR);
     ~WebSocketWrapper();

    QWebSocket *getSocket() const;
    qint64 sendTextMessage(const QString& message);
    const QString getLastMessage();

signals:
    void sendTextMessageSignal(const QString& message);

public slots:
    void socketDisconnected();
    void socketConnected();
    void sendTextMessageSlot(const QString& message);

private:
    QWebSocket* m_pSocket;
    QString m_lastMessage;
    QMutex* m_pMutex;
    bool m_connected;
};

#endif // WEBSOCKETWRAPPER_H
