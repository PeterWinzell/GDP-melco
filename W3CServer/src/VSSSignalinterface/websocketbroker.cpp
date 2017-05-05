#include "websocketbroker.h"
#include "logger.h"
#include "errors/errorresponse.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QVector>
#include <QFile>
#include <QCoreApplication>
#include <QRegularExpression>


WebSocketBroker::WebSocketBroker(const QString& vssDir, const QString &vssName, const QString &brokerUrl, QObject *parent) :
    QObject(parent), m_brokerUrl(brokerUrl)
{
    loadJson(vssDir + "/" + vssName + ".json");
    loadTempSignalList(vssDir + "/" + vssName + ".vsi");

    connect(&m_webSocket, &QWebSocket::connected, this, &WebSocketBroker::onConnected);
    connect(this, &WebSocketBroker::sendMessageSignal, this, &WebSocketBroker::sendMessageSlot, Qt::QueuedConnection);

    m_webSocket.open(QUrl(brokerUrl));
}

int WebSocketBroker::getSignalValue(const QString& path, QJsonArray& values)
{
    // Check if path is single leaf.
    // Check if path is single branch.
    // Check if path contains asterisk.
    // Check if path contains asterisk and leaf.
    QJsonArray paths = parseGetPath(path);
    DEBUG("WebSocketBroker",path);

    QMutexLocker locker(&m_mutex);
    if(!checkOpenDSConnection()) return ErrorReason::gateway_timeout;
    // Check if signal exists.
    if(!checkSignals(paths, true)) { return ErrorReason::invalid_path; }

    DEBUG("WebSocketBroker","path exist");

    // Create message to send
    QJsonObject message;
    message.insert("get", paths);
    QJsonDocument jsonDoc(message);

    // Lock and send message.

    sendMessage(jsonDoc.toJson());

    QEventLoop messageLoop;
    connect(this, &WebSocketBroker::messageReceived, &messageLoop, &QEventLoop::quit);
    QTimer::singleShot(m_timeout, &messageLoop, &QEventLoop::quit);
    messageLoop.exec();

    jsonDoc = QJsonDocument(m_receivedMessage);

    // TODO Add check of Success in response.
    // Could just return false, instead of array, if error?
    if(jsonDoc.object()["get"].isArray())
    {
        values = jsonDoc.object()["get"].toArray();
        DEBUG(" WebSocketBroker", "values are set");
        return 0;
    }

    DEBUG("WebSocketBroker", " there is no array in response ");
    return ErrorReason::bad_gateway;
}

int WebSocketBroker::setSignalValue(const QString& path, const QVariant& values)
{
    QJsonArray paths = parseSetPath(path, values.toJsonValue());

    QMutexLocker locker(&m_mutex);
    if(!checkOpenDSConnection()) return ErrorReason::gateway_timeout;
    // Check if signal exists.
    if(!checkSignals(paths, false)) {
        return ErrorReason::invalid_path;
    }

    // Create message to send
    QJsonObject message;
    message.insert("set", paths);
    QJsonDocument jsonDoc(message);

    sendMessage(jsonDoc.toJson());

    QEventLoop messageLoop;
    connect(this, &WebSocketBroker::messageReceived, &messageLoop, &QEventLoop::quit);
    QTimer::singleShot(m_timeout, &messageLoop, &QEventLoop::quit);
    messageLoop.exec();

    jsonDoc = QJsonDocument(m_receivedMessage);

    qDebug() << jsonDoc;

    // Should return false, or 0 if not bool. So should work without checking if bool
    if (jsonDoc.object()["set"].toBool())
        return 0;
    else
        return ErrorReason::bad_gateway;
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

void WebSocketBroker::sendMessage(const QString message)
{
    // Need to reset so that we dont accidentally use old message. Change to timeout errormsg?
    m_receivedMessage = QJsonObject();

    emit sendMessageSignal(message);
}

void WebSocketBroker::sendMessageSlot(const QString message)
{
    m_webSocket.sendTextMessage(message);
    m_webSocket.flush();
}

void WebSocketBroker::onConnected()
{
    WARNING("","Connected to OpenDSAdapter");
    connect(&m_webSocket, &QWebSocket::textMessageReceived, this, &WebSocketBroker::onTextMessageReceived);
}

void WebSocketBroker::onTextMessageReceived(QString message)
{
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(message.toUtf8(), &parseError);
    if(parseError.error != QJsonParseError::NoError)
    {
        m_receivedMessage = QJsonObject(); // Change to error.
    }
    else { m_receivedMessage = doc.object(); }

    emit messageReceived();
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

void WebSocketBroker::loadTempSignalList(const QString &vssFile)
{
    QRegularExpression regex(QString("^(\\S+) \\d+"));

    QFile file(vssFile);
    file.open(QFile::ReadOnly);
    if(!file.exists())
    {
        WARNING("Server", "The flattened vsi signal file could not be found for the Signal Broker!");
        file.close();
        return;
    }
    while(!file.atEnd())
    {
        QRegularExpressionMatch match = regex.match(file.readLine());
        if(match.hasMatch()) { m_tempSignalList.append(match.captured(1).trimmed()); }
    }
}

bool WebSocketBroker::checkOpenDSConnection(){
    if(m_webSocket.isValid()) return true;
    WARNING("WebSocketBroker","No connection to OpenDS Adapter. Trying to reconnect.");
    m_webSocket.open(QUrl(m_brokerUrl));
    return false;
}

QJsonArray WebSocketBroker::parseGetPath(const QString& path)
{
    QStringList sl = splitPath(path);
    QJsonArray values;

    if(sl.length() == 1)
        foreach(auto const item, getPath(sl.first()))
    {
        QJsonObject value;
        value.insert(item,"");
        values.append(value);
    }
    else
        foreach(auto const item, getPath(sl.first(), sl.last()))
    {
        QJsonObject value;
        value.insert(item,"");
        values.append(value);
    }

    return values;
}

QJsonArray WebSocketBroker::parseSetPath(const QString& path, const QJsonValue &setValues)
{
    QStringList sl = splitPath(path);
    QJsonArray values;

    if(setValues.isArray())
    {
        foreach(QJsonValue const val, setValues.toArray())
        {
            QJsonObject value;
            QJsonObject obj = val.toObject();

            QString valuePath = sl.first() + "." + obj.keys().first();
            QJsonValue tmp = obj.value(obj.keys().first());

            if(tmp.isDouble())
            {
                value.insert(valuePath, tmp.toDouble());
            }
            else if(tmp.isBool())
            {
                value.insert(valuePath, tmp.toBool());
            }
            else
            {
                value.insert(valuePath, tmp.toString());
            }

            values.append(value);
        }
    }
    else
    {
        QJsonObject value;

        if(setValues.isDouble())
        {
            value.insert(path, setValues.toDouble());
        }
        else if(setValues.isBool())
        {
            value.insert(path, setValues.toBool());
        }
        else
        {
            value.insert(path, setValues.toString());
        }

        values .append(value);
    }

    return values;
}

bool WebSocketBroker::checkSignals(const QJsonArray &paths, bool getOrSet)
{
    foreach (QJsonValue const &obj, paths)
    {
        foreach (QJsonValue key, obj.toObject().keys())
        {
            QJsonObject signal = getVSSNode(key.toString());
            if(signal.isEmpty())
            {
                WARNING("Server", QString("Requested signal was not found : %1").arg(key.toString()));
                return false;
            }

            if(getOrSet)
            {
                // TODO Check if the signal is ok to get
            }
            else
            {
                // TODO Check if the signal is ok to set
                // Check if set is correct type
            }
        }
    }
    return true;
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
