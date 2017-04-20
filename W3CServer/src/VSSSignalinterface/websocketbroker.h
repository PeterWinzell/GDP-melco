#ifndef WEBSOCKETBROKER_H
#define WEBSOCKETBROKER_H

#include <QMutex>
#include <QVariant>
#include <QJsonObject>
#include <QWebSocket>
#include "vsssignalinterface.h"

class WebSocketBroker : public VSSSignalInterface
{
public:
    WebSocketBroker(const QString &vssFile);

    struct JsonNode
    {
        QJsonObject json;
        QString key;
        bool isBranch;
    };

    QJsonArray getSignalValue(const QString& path);
    QString setSignalValue(const QString& path, QVariant value);
    QJsonObject getVSSNode(const QString& path);
    QJsonObject getVSSTree(const QString& path);

signals:
    void closed();

private slots:
    void onConnected();
    void onTextMessageReceived(QString message);


private:
    void loadJson(const QString& fileName);
    void getTreeNodes(QJsonObject& tree, QStringList& path, QVector<JsonNode>& nodes);
    void removeAllKeysButOne(QJsonObject &json, const QString &filter);
    void removeOne(QJsonObject &json, const QString &filter);
    void createJsonVssTree(QVector<JsonNode>& nodes, QJsonObject &json);

    void sendMessage(QString& message);

    void loadTempSignalList();

    QString parseGetPath(QString path);
    QString parseSetPath(QString path, QJsonValue values);

    QStringList splitPath(QString path);

    QStringList getPath(QString startsWith);
    QStringList getPath(QString startsWith, QString endsWith);



    QMutex m_mutex;

    QWebSocket m_webSocket;

    QJsonObject m_vssTree;
    QJsonObject m_vssTreeNode;

    bool m_messageReceivedFromBroker = false;
    QJsonObject m_receivedMessage;

    QStringList m_tempSignalList;
};

#endif // WEBSOCKETBROKER_H
