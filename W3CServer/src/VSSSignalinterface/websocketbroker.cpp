#include "websocketbroker.h"
#include "logger.h"
#include <QJsonDocument>
#include <QVector>
#include <QFile>

WebSocketBroker::WebSocketBroker(const QString& vssFile)
{
    loadJson(vssFile);

    connect(&m_webSocket, &QWebSocket::connected, this, &WebSocketBroker::onConnected);
    //connect(&m_webSocket, &QWebSocket::disconnected, this, &WebSocketBroker::closed);

    m_webSocket.open(QUrl("ws://localhost:42"));
}

QString WebSocketBroker::getSignalValue(const QString& path)
{
    QMutexLocker locker(&m_mutex);

    Q_UNUSED(path);

    QString result = "not implemented";

    return result;
}

qint8 WebSocketBroker::setSignalValue(const QString& path, QVariant value)
{
    QMutexLocker locker(&m_mutex);

    Q_UNUSED(path);
    Q_UNUSED(value);

    qint8 result = 0;

    return result;
}

QJsonObject WebSocketBroker::getVSSNode(const QString& path)
{

    // clear old
    m_vssTreeNode = QJsonObject();

    if (m_vssTree.empty())
    {
        DEBUG("Server","Empty VSS tree. Returning empty node.");
        return m_vssTreeNode;
    }
    else if (path == "")
    {
        DEBUG("Server","Empty path. Returning full tree.");
        return m_vssTree;
    }
    else
    {
        QStringList tokens = path.split('.');

        // Make a copy of the full tree
        QJsonObject tree = m_vssTree;

        QVector<JsonNode> nodes;
        getTreeNodes(tree, tokens, nodes);

        QJsonObject obj;
        createJsonVssTree(nodes, obj);

        return m_vssTreeNode;
    }
}

QJsonObject WebSocketBroker::getVSSTree(const QString& path)
{
    QString p = path;
    QJsonObject tree;
    return tree;
}


void WebSocketBroker::onConnected()
{
    connect(&m_webSocket, &QWebSocket::textMessageReceived, this, &WebSocketBroker::onTextMessageReceived);
    m_webSocket.sendTextMessage(QStringLiteral("Hello, world!"));
}

void WebSocketBroker::onTextMessageReceived(QString message)
{
    INFO("TEST",message);
    m_webSocket.close();
}














void WebSocketBroker::getTreeNodes(QJsonObject& tree, QStringList& path, QVector<JsonNode>& nodes)
{
    if (path.length() > 0)
    {
        QString key = path[0];
        JsonNode node;
        node.isBranch = path.length() > 1;
        node.key = key;

        if (node.isBranch)
        {
            removeAllKeysButOne(tree, key);
        }

        tree = tree.value(key).toObject();

        QJsonObject treeCopy = tree;

        if (node.isBranch)
        {
            removeOne(treeCopy, "children");
            tree = tree.value("children").toObject();
        }

        node.json = treeCopy;

        nodes.push_front(node);

        path.removeFirst();
        getTreeNodes(tree, path, nodes);
    }
}

void WebSocketBroker::createJsonVssTree(QVector<JsonNode>& nodes, QJsonObject& json)
{

    if (nodes.size() > 0)
    {
        JsonNode node = nodes.first();
        QJsonObject obj;

        if (node.isBranch)
        {
            node.json.insert("children", json);
            obj.insert(node.key, node.json);
        }
        else
        {
            obj.insert(node.key, node.json);
        }

        nodes.removeFirst();
        createJsonVssTree(nodes, obj);
    }
    else
    {
        m_vssTreeNode = json;
    }
}

void WebSocketBroker::removeAllKeysButOne(QJsonObject& json, const QString& filter)
{
    foreach (QString key, json.keys())
    {
        if (filter != key)
        {
            json.remove(key);
        }
    }
}

void WebSocketBroker::removeOne(QJsonObject& json, const QString& filter)
{
    foreach (QString key, json.keys())
    {
        if (filter == key)
        {
            json.remove(key);
        }
    }
}

void WebSocketBroker::loadJson(const QString &fileName)
{
    QFile jsonFile(fileName);
    QJsonDocument doc;

    if (jsonFile.exists())
    {
        jsonFile.open(QFile::ReadOnly);
        doc = QJsonDocument().fromJson(jsonFile.readAll());
    }
    else
    {
        WARNING("Server",QString("VSS File %1 not found!").arg(fileName));
    }

    m_vssTree = doc.object();
}
