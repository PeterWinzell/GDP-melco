#ifndef WEBSOCKETWRAPPER_H
#define WEBSOCKETWRAPPER_H

#include <QObject>
#include <QWebSocket>
#include <QHash>

class WebSocketWrapper : public QObject
{
    Q_OBJECT
public:
    explicit WebSocketWrapper(QWebSocket* socket, QMutex* mutex, QObject *parent = Q_NULLPTR);

    QWebSocket *getSocket() const;
    qint64 sendTextMessage(const QString& message);
    const QString getLastMessage();

signals:
   void SendTextMessage(QString message);
public slots:

private:
    QWebSocket* m_pSocket;
    QString m_lastMessage;
    QMutex* m_pMutex;
};

#endif // WEBSOCKETWRAPPER_H
