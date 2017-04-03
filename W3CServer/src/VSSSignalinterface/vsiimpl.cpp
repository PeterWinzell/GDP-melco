#include "vsiimpl.h"

#include <QDebug>
#include <QJsonObject>
#include <QFile>
#include <QRegularExpression>
#include <QJsonDocument>

vsi_handle VSIImpl::handle;
VSIImpl::VSIImpl(const QString &vssDir, const QString &vssName)
{
    VSIImpl::handle = vsi_initialize ( false );

    QString vsiFile = vssDir + "/" + vssName + ".vsi";
    QString vssjson = vssDir + "/" + vssName + ".json";

    loadVSSFile(vsiFile);
    loadJson(vssjson);

    vsi_VSS_import(handle, vsiFile.toStdString().c_str());
}
void VSIImpl::loadJson(const QString &fileName)
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

void VSIImpl::loadVSSFile(const QString& vsiFile)
{
    QFile file(vsiFile);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Couldnt read VSS File!";
        return;
    }
    QRegularExpression re("^(([a-zA-Z0-9]*\\.?)+)\\s(\\d{0,3})\\s?(\\d{0,3})");
    while (!file.atEnd())
    {
        QByteArray line = file.readLine();
        QRegularExpressionMatch match = re.match(line);
        if(line[0] != '1')
            if(match.hasMatch())
            {
                // Name, Key, Private Key
                //qDebug() << match.captured(1) <<  match.captured(3) << match.captured(4);
                m_nameKeyMap[match.captured(1)] = match.captured(3).toInt();
            }
    }
}

QString VSIImpl::getSignalValue(const QString& path)
{
    vsi_result result = getResult(path);
    if(result.status == -1) { return ""; }

    char* tempReadBuffer = new char[50] ;
    memset(tempReadBuffer, 0, sizeof(tempReadBuffer));
    result.data = tempReadBuffer;

    vsi_get_newest_signal(VSIImpl::handle, &result);

    if(result.status != 0)
    {
        qDebug() << "Failed to fetch signal data. STATUS: " << result.status << "   DATA:" <<  result.data;
        return "";
    }
    qDebug() << "Sucessfully fetched data " << result.data;
    return QString(result.data);
}

qint8 VSIImpl::setSignalValue(const QString& path, QVariant value)
{
    vsi_result result = getResult(path);
    if(result.status == -1) { return -1; }

    //TODO  need to convert QVariant to some value
    char* data = (char*)sm_malloc (20);
    strncpy ( data, "90", 20);
    //


    result.data = data;
    result.dataLength = strlen(result.data);

    vsi_fire_signal(VSIImpl::handle, &result);
    if(result.status != 0)
    {
        qDebug() << "Failed to set new signal data. STATUS: " << result.status;
        return result.status;
    }
    qDebug() << "Sucessfully set new signal data";
    return result.status;
}

QJsonObject VSIImpl::getVSSTree(const QString& path)
{
    QString p = path;
    QJsonObject tree;
    return tree;
}

QJsonObject VSIImpl::getVSSNode(const QString& path)
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

void VSIImpl::getTreeNodes(QJsonObject& tree, QStringList& path, QVector<JsonNode>& nodes)
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

void VSIImpl::createJsonVssTree(QVector<JsonNode>& nodes, QJsonObject& json)
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

void VSIImpl::removeAllKeysButOne(QJsonObject& json, const QString& filter)
{
    foreach (QString key, json.keys())
    {
        if (filter != key)
        {
            json.remove(key);
        }
    }
}

void VSIImpl::removeOne(QJsonObject& json, const QString& filter)
{
    foreach (QString key, json.keys())
    {
        if (filter == key)
        {
            json.remove(key);
        }
    }
}



vsi_result VSIImpl::getResult(const QString& path)
{
    vsi_result result;
    result.domainId = 0;
    result.status = 0;

    if(m_nameKeyMap.contains(path))
    {
        result.signalId = m_nameKeyMap[path];
    }
    else
    {
        qDebug() << "Failed to get signal key" << path;
        result.signalId = -1;
        result.status = -1;
    }
    return result;
}
