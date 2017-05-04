#ifndef WEBSOCKETBROKER_H
#define WEBSOCKETBROKER_H

#include <QMutex>
#include <QVariant>
#include <QJsonObject>
#include <QWebSocket>
#include "vsssignalinterface.h"
#include <QTimer>
#include <QEventLoop>

class WebSocketBroker : public QObject, public VSSSignalInterface
{
    Q_OBJECT
public:
    WebSocketBroker(const QString &vssDir, const QString &vssName, const QString &brokerUrl = "ws://localhost:8008", QObject *parent = Q_NULLPTR);

    struct JsonNode
    {
        QJsonObject json;
        QString key;
        bool isBranch;
    };
    bool getSignalValue(const QString& path, QJsonArray& values);
    int setSignalValue(const QString& path, const QVariant &value);
    QJsonObject getVSSNode(const QString& path);
    QJsonObject getVSSTree(const QString& path);

signals:
    void closed();
    void messageReceived();
    void sendMessageSignal(const QString message);

private slots:
    void onConnected();
    void onTextMessageReceived(QString message);
    void sendMessageSlot(const QString message);

private:
    void loadJson(const QString& fileName);
    void getTreeNodes(QJsonObject& tree, QStringList& path, QVector<JsonNode>& nodes);
    void removeAllKeysButOne(QJsonObject &json, const QString &filter);
    void removeOne(QJsonObject &json, const QString &filter);
    void createJsonVssTree(QVector<JsonNode>& nodes, QJsonObject &json);

    void sendMessage(const QString message);

    void loadTempSignalList(const QString &vssFile);
    bool checkOpenDSConnection();
    QJsonArray parseGetPath(const QString &path);
    QJsonArray parseSetPath(const QString &path, const QJsonValue &values);
    bool checkSignals(const QJsonArray &paths, bool getOrSet);

    QStringList splitPath(QString path);

    QStringList getPath(QString startsWith);
    QStringList getPath(QString startsWith, QString endsWith);

    QMutex m_mutex;
    long m_timeout = 150;
    QString m_brokerUrl;
    QWebSocket m_webSocket;

    QJsonObject m_vssTree;
    QJsonObject m_vssTreeNode;

    QJsonObject m_receivedMessage;

    QStringList m_tempSignalList;
};

#endif // WEBSOCKETBROKER_H
