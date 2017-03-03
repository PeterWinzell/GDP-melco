#ifndef WEBSOCKETWRAPPER_H
#define WEBSOCKETWRAPPER_H

#include <QObject>
#include <QWebSocket>

class WebSocketWrapper : public QObject
{
    Q_OBJECT
public:
    explicit WebSocketWrapper(QWebSocket* socket, QObject *parent = Q_NULLPTR);
    virtual ~WebSocketWrapper();

    QWebSocket *getSocket() const;
    qint64 sendTextMessage(const QString& message);
    const QString getLastMessage();

signals:

public slots:

private:
    QWebSocket* m_pSocket;
    QString m_lastMessage;
};

#endif // WEBSOCKETWRAPPER_H
