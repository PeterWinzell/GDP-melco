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
    // qDebug()  << "VSSSignalInterfaceImpl::updateValue: " << "\n\ttype: " << type << "\n\tvalue: " << value;
    QMutex mutex;
    QMutexLocker locker(&mutex);

    switch (type)
    {
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
    QJsonDocument doc;

    if (jsonFile.exists())
    {
        jsonFile.open(QFile::ReadOnly);
        doc = QJsonDocument().fromJson(jsonFile.readAll());
    }
    else
    {
        qWarning() << "Vss file " << fileName << " not found!";
    }

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

qint8 VSSSignalInterfaceImpl::setSignalValue(const QString& path, QVariant value)
{
    QMutex mutex;
    QMutexLocker locker(&mutex);

    qint8 result = 0;

    qDebug() << "setSignalValue: path = " << path;

    if(path == "Signal.Drivetrain.InternalCombustionEngine.RPM")
    {
        m_rpm = value.toString();

        qDebug() << "m_rpm = " << m_rpm;
    }
    else if (path == "Signal.Drivetrain.Transmission.Speed")
    {
        m_speed = value.toString();

        qDebug() << "m_speed = " << m_speed;
    }

    return result;
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
    QString p = path;
    QJsonObject tree;
    return tree;
}
