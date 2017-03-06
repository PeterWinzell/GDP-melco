#ifndef VSSSIGNALINTERFACEIMPL_H
#define VSSSIGNALINTERFACEIMPL_H

#include <QObject>
#include "vsssignalinterface.h"
#include <QMutex>

class VSSSignalInterfaceImpl : public VSSSignalInterface
{
public:
    VSSSignalInterfaceImpl();
    QString getSignalValue(const QString& path);
    qint8   setSignalValue(const QString& path);
    QJsonObject getVSSNode(const QString& path);
    QJsonObject getVSSTree(const QString& path);

private:
    QJsonDocument loadJson(const QString& fileName);

private:
    QString rpm;
    QString speed;
    QMutex mutex;
};

#endif // VSSSIGNALINTERFACEIMPL_H
