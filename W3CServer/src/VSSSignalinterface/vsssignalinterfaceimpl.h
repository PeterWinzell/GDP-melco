#ifndef VSSSIGNALINTERFACEIMPL_H
#define VSSSIGNALINTERFACEIMPL_H

#include <QObject>
#include <QMutex>
#include "vsssignalinterface.h"
#include <QMutex>

class VSSSignalInterfaceImpl : public VSSSignalInterface
{
public:
    VSSSignalInterfaceImpl(const QString &vssFile);
    QString getSignalValue(const QString& path);
    qint8   setSignalValue(const QString& path);
    QJsonObject getVSSNode(const QString& path);
    QJsonObject getVSSTree(const QString& path);

    struct JsonNode
    {
        QJsonObject json;
        QString key;
        bool isBranch;
    };

private:
    void loadJson(const QString& fileName);
    void getTreeNodes(QJsonObject& tree, QStringList& path, QVector<JsonNode>& nodes);
    void removeAllKeysButOne(QJsonObject &json, const QString &filter);
    void removeOne(QJsonObject &json, const QString &filter);
    void createJsonVssTree(QVector<JsonNode>& nodes, QJsonObject &json);

private:
    QString m_rpm;
    QString m_speed;
    QMutex m_mutex;

    QJsonObject m_vssTree;
    QJsonObject m_vssTreeNode;

};

#endif // VSSSIGNALINTERFACEIMPL_H
