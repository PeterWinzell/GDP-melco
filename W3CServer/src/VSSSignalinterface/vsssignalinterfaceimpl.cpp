#include "vsssignalinterfaceimpl.h"
#include <QJsonObject>
#include <QString>
#include <QMutex>
#include <QJsonDocument>
#include <QFile>
#include <QDir>
#include <QDebug>

VSSSignalInterfaceImpl::VSSSignalInterfaceImpl(const QString& vssFile)
{
    rpm = "3000";
    speed = "70";

    loadJson(vssFile);
}

void VSSSignalInterfaceImpl::loadJson(const QString &fileName)
{
    QFile jsonFile(fileName);

    if (jsonFile.exists())
    {
        jsonFile.open(QFile::ReadOnly);
    }
    else
    {
        qWarning() << "Vss file " << fileName << " not found!";
    }

    QJsonDocument doc = QJsonDocument().fromJson(jsonFile.readAll());

    m_vssTree = doc.object();
}

QString VSSSignalInterfaceImpl::getSignalValue(const QString& path)
{
    QString result = "not implemented";

    if(path == "Signal.Drivetrain.InternalCombustionEngine.RPM")
    {
        mutex.lock();
        result = rpm;
        mutex.unlock();

    }
    else if (path == "Signal.Drivetrain.Transmission.Speed")
    {
        mutex.lock();
        result = speed;
        mutex.unlock();
    }

    return result;
}

qint8 VSSSignalInterfaceImpl::setSignalValue(const QString& path)
{
    return 0;
}

QJsonObject VSSSignalInterfaceImpl::getVSSNode(const QString& path)
{

    // clear old
    m_vssTreeNode = QJsonObject();

    if (m_vssTree.empty())
    {
        qDebug() << "Empty vss tree! Returning empty node.";
        return m_vssTreeNode;
    }
    else if (path == "")
    {
        qDebug() << "Empty path, returning full tree.";
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

void VSSSignalInterfaceImpl::getTreeNodes(QJsonObject& tree, QStringList& path, QVector<JsonNode>& nodes)
{
    if (path.length() > 0)
    {
        QString key = path[0];

//        if (key != "*")
//        {
        removeAllKeysButOne(tree, key);
//        }

        tree = tree.value(key).toObject();

        QJsonObject treeCopy = tree;
        removeOne(treeCopy, "children");

        JsonNode node;
        node.isBranch = false;
        node.json = treeCopy;
        node.key = key;

        if (path.length() > 1)
        {
            tree = tree.value("children").toObject();
            node.isBranch = true;
        }

        nodes.push_front(node);

        path.removeFirst();
        getTreeNodes(tree, path, nodes);
    }
}

void VSSSignalInterfaceImpl::createJsonVssTree(QVector<JsonNode>& nodes, QJsonObject& json)
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

void VSSSignalInterfaceImpl::removeAllKeysButOne(QJsonObject& json, const QString& filter)
{
    foreach (QString key, json.keys())
    {
        if (filter != key)
        {
            json.remove(key);
        }
    }
}

void VSSSignalInterfaceImpl::removeOne(QJsonObject& json, const QString& filter)
{
    foreach (QString key, json.keys())
    {
        if (filter == key)
        {
            json.remove(key);
        }
    }
}

QJsonObject VSSSignalInterfaceImpl::getVSSTree(const QString& path)
{
    QJsonObject tree;
    return tree;
}