#ifndef W3CTESTCLIENT_H
#define W3CTESTCLIENT_H

#include <QtCore/QObject>
#include <QtWebSockets/QWebSocket>
#include <QtNetwork/QSslError>
#include <QtCore/QList>
#include <QtCore/QString>
#include <QtCore/QUrl>


QT_FORWARD_DECLARE_CLASS(QWebSocket)

class W3cTestClient : public QObject
{
    Q_OBJECT
public:
    explicit W3cTestClient(const QUrl &url, QObject *parent = Q_NULLPTR);

    void RunSubscribeUnsubscribeAllTest();
    void RunSubscribeUnsubscribeTest();

private Q_SLOTS:
    void onConnected();
    void onTextMessageReceived(QString message);
    void onSslErrors(const QList<QSslError> &errors);

    //timer slots
    void unsubscribe();
    void unsubscribeAll();
private:
    int m_subscriptionid;
    QWebSocket m_webSocket;
};

#endif // W3CTESTCLIENT_H
