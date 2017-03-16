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
    void RunGetVssTest();
    void RunAuthorizeTest();

    enum class TestCase
    {
        SUBSCRIBE_UNSUBSCRIBE,
        SUBSCRIBEALL_UNSUBSCRIBEALL,
        AUTHORIZE_SUCCESS,
        GET_VSS,
        SET_GET
    };

    void setTest(TestCase test);

private Q_SLOTS:
    void onConnected();
    void onTextMessageReceived(QString message);
    void onSslErrors(const QList<QSslError> &errors);

    //timer slots
    void unsubscribe();
    void unsubscribeAll();

private:
    void close(int result);
    QString m_unsubscribeCachedSubscriptionId; // keep track of this to perform unsubscribe.
    QWebSocket m_webSocket;
    QUrl m_url;
    TestCase m_test;

};

#endif // W3CTESTCLIENT_H
