#include "vsssignalinterfaceimpl.h"
#include <QJsonObject>
#include <QString>
#include <QMutexLocker>
#include <QJsonDocument>
#include <QPointer>
#include <QFile>
#include <QDir>
#include <QDebug>
#include <QJsonArray>

VSSSignalInterfaceImpl::VSSSignalInterfaceImpl(const QString& vssFile)
{
    m_rpm = "3000";
    m_speed = "70";

    loadJson(vssFile);
}

void VSSSignalInterfaceImpl::updateValue(CarSignalType type, QString value)
{
    qDebug()  << "VSSSignalInterfaceImpl::updateValue: " << "\n\ttype: " << type << "\n\tvalue: " << value;
    QMutex mutex;
    QMutexLocker locker(&mutex);

    switch (type) {
    case CarSignalType::RPM:
        m_rpm = value;
        break;
    case CarSignalType::Speed:
        m_speed = value;
        break;
    default:
        break;
    }
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
    QMutex mutex;
    QMutexLocker locker(&mutex);

    QString result = "not implemented";

    if(path == "Signal.Drivetrain.InternalCombustionEngine.RPM")
    {

        result = m_rpm;

    }
    else if (path == "Signal.Drivetrain.Transmission.Speed")
    {

        result = m_speed;

    }

    return result;
}

qint8 VSSSignalInterfaceImpl::setSignalValue(const QString& path)
{
    QString p = path;
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
            //            QStringList keys = node.json.keys();
            //            QVector<JsonNode> jsonObjects;

            //            // sort key order to get children last

            //            foreach (QString key, keys)
            //            {
            //                JsonNode copyNode;
            //                copyNode.key = key;
            //                copyNode.json = node.json.value(key).toObject();
            //                jsonObjects.push_back(copyNode);
            //            }

            //            foreach (QString key, keys)
            //            {
            //                node.json.remove(key);
            //            }

            //            foreach (JsonNode n, jsonObjects)
            //            {
            //                node.json.insert(n.key, n.json);
            //            }

            //            // will be placed last

            node.json.insert("children", json);

            // insert the reordered object
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
    QString p = path;
    QJsonObject tree;
    return tree;
}
