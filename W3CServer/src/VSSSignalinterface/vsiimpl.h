#ifndef VSIIMPL_H
#define VSIIMPL_H
extern "C" {
    #include "../../../lib/vsi/vsi.h"
    #include "../../../lib/vsi/vsi_core_api.h"
}
//#undef signal
//#include <qobjectdefs.h>

#include <QObject>
#include <QMutex>
#include <QMap>
#include "vsssignalinterface.h"



class VSIImpl : public VSSSignalInterface
{
public:
    VSIImpl(const QString &vssDir, const QString &vssName);
    QString getSignalValue(const QString& path);
    qint8   setSignalValue(const QString& path, QVariant value);
    QJsonObject getVSSNode(const QString& path);
    QJsonObject getVSSTree(const QString& path);

    struct JsonNode
    {
        QJsonObject json;
        QString key;
        bool isBranch;
    };
private:
    QJsonObject m_vssTree;
    QJsonObject m_vssTreeNode;

    static vsi_handle handle;
    QMap<QString, int> m_nameKeyMap;

    void loadJson(const QString& fileName);
    void loadVSSFile(const QString &vsiFile);
    void getTreeNodes(QJsonObject& tree, QStringList& path, QVector<JsonNode>& nodes);
    void removeAllKeysButOne(QJsonObject &json, const QString &filter);
    void removeOne(QJsonObject &json, const QString &filter);
    void createJsonVssTree(QVector<JsonNode>& nodes, QJsonObject &json);
    vsi_result getResult(const QString& path);
};

#endif // VSIIMPL_H
