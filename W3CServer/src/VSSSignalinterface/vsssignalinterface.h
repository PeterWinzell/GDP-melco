#ifndef VSSSIGNALINTERFACE_H
#define VSSSIGNALINTERFACE_H

#include <QJsonObject>

class VSSSignalInterface
{
public:
    virtual int getSignalValue(const QString& path, QJsonArray& values) = 0;
    virtual int setSignalValue(const QString& path, const QVariant &value) = 0;
    virtual QJsonObject getVSSNode(const QString& path) = 0;
    virtual QJsonObject getVSSTree(const QString& path) = 0;
};
#endif // VSSSIGNALINTERFACE_H
