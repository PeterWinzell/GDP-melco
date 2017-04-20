#include "websocketbroker.h"
#include "logger.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QVector>
#include <QFile>
#include <QCoreApplication>
#include <QRegularExpression>

WebSocketBroker::WebSocketBroker(const QString& vssFile)
{
    loadJson(vssFile);
    loadTempSignalList();

    connect(&m_webSocket, &QWebSocket::connected, this, &WebSocketBroker::onConnected);
    //connect(&m_webSocket, &QWebSocket::disconnected, this, &WebSocketBroker::closed);
    m_webSocket.open(QUrl("ws://localhost:42"));
}

bool WebSocketBroker::getSignalValue(const QString& path, QJsonArray& values)
{
    QMutexLocker locker(&m_mutex);
    m_receivedMessage = QJsonObject();

    // Check if path is single leaf.
    // Check if path is single branch.
    // Check if path contains asterisk.
    // Check if path contains asterisk and leaf.  
    QString msg = parseGetPath(path);

    sendMessage(msg);

    // TODO Need to implement some kind of timer here.
    while(!m_messageReceivedFromBroker)
    {
        //DEBUG("TEST","Waiting for response...");
    }

    QJsonDocument jsonDoc(m_receivedMessage);

    // TODO Add check of Success in response.

    if(jsonDoc.object()["get"].isArray())
    {
        values = jsonDoc.object()["get"].toArray(); // Test this
        //foreach (QJsonValue value, jsonDoc.object()["get"].toArray()) {
        //    values.append(value);
        //}
        return true;
    }
    return false;
}

bool WebSocketBroker::setSignalValue(const QString& path, const QVariant& value)
{
    QMutexLocker locker(&m_mutex);
    m_receivedMessage = QJsonObject();

    QString msg = parseSetPath(path, value.toJsonValue());

    sendMessage(msg);

    // TODO Need to implement some kind of timer here.
    while(!m_messageReceivedFromBroker)
    {
        //DEBUG("TEST","Waiting for response...");
    }

    QJsonDocument jsonDoc(m_receivedMessage);

    // Should return false, or 0 if not bool. So should work without checking if bool
    return jsonDoc.object()["set"].toBool();
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


void WebSocketBroker::sendMessage(QString& message)
{
    m_messageReceivedFromBroker = false;
    m_webSocket.sendTextMessage(message);
    m_webSocket.flush();
}

void WebSocketBroker::onConnected()
{
    connect(&m_webSocket, &QWebSocket::textMessageReceived, this, &WebSocketBroker::onTextMessageReceived);
}

void WebSocketBroker::onTextMessageReceived(QString message)
{
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(message.toUtf8(), &parseError);
    if(parseError.error != QJsonParseError::NoError)
    {
        m_receivedMessage = doc.object(); // Change to error.
        m_messageReceivedFromBroker = true;
        return;
    }
    m_receivedMessage = doc.object();
    m_messageReceivedFromBroker = true;
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

void WebSocketBroker::loadTempSignalList()
{
    QRegularExpression regex(QString("^(\\S+) \\d+"));

    QFile file(QCoreApplication::applicationDirPath() + "/vss_rel_1.vsi");
    file.open(QFile::ReadOnly);

    while(!file.atEnd())
    {
        QRegularExpressionMatch match = regex.match(file.readLine());
        if(match.hasMatch()) { m_tempSignalList.append(match.captured(1).trimmed()); }
    }
}

QString WebSocketBroker::parseGetPath(QString path)
{
    QStringList sl = splitPath(path);

    QJsonObject msg;
    QJsonArray values;


    if(sl.length() == 1)
        foreach(auto item, getPath(sl.first()))
    {
        QJsonObject value;
        value.insert(item,"");
        values.append(value);
    }
    else
        foreach(auto item, getPath(sl.first(), sl.last()))
    {
        QJsonObject value;
        value.insert(item,"");
        values.append(value);
    }
    msg.insert("get", values);

    QJsonDocument jsonDoc(msg);
    return jsonDoc.toJson();
}

QString WebSocketBroker::parseSetPath(QString path, QJsonValue setValues)
{
    QStringList sl = splitPath(path);

    QJsonObject msg;
    QJsonArray values;

    if(setValues.isArray())
    {
        foreach(QJsonValue val, setValues.toArray())
        {
            QJsonObject value;
            QString valuePath = sl.first() + "." + val.toObject().keys().first();

            if(val.isDouble())
               value.insert(valuePath, val.toDouble());
            else if(val.isBool())
                value.insert(valuePath, val.toBool());
            else
                value.insert(valuePath, val.toString());



            // TODO Check different types of values and make this look nicer.

            value.insert(sl.first() + "." + val.toObject().keys().first(), val.toObject().keys().first().toDouble());

            values.append(value);
        }
    }
    else
    {
        // TODO ... same here.
        QJsonObject value;
        value.insert(sl.first(), setValues.toDouble());
        values .append(value);
    }

    msg.insert("set", values);

    QJsonDocument jsonDoc(msg);
    return jsonDoc.toJson();
}

QStringList WebSocketBroker::getPath(QString startsWith)
{
    return getPath(startsWith, "");
}

QStringList WebSocketBroker::getPath(QString startsWith, QString endsWith)
{
    return m_tempSignalList.filter(QRegularExpression(QString("^%1.*%2$").arg(startsWith, endsWith)));
}

QStringList WebSocketBroker::splitPath(QString path)
{
    QStringList sl;
    int i = path.lastIndexOf("*");
    if(i == -1) { sl << path; return sl; }

    foreach(QString item, path.split("*"))
    {
        // Trim '.' from items.
        if(item.startsWith(".")) { item = item.remove(0,1); }
        if(item.endsWith(".")) { item = item.remove(item.length() -1, 1); }

        sl << item;
    }
    return sl;
}
