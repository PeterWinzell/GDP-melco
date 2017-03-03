#ifndef VSSSIGNALINTERFACEIMPL_H
#define VSSSIGNALINTERFACEIMPL_H

#include <QObject>
#include <QMutex>
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
    QString m_rpm;
    QString m_speed;
    QMutex m_mutex;
};

#endif // VSSSIGNALINTERFACEIMPL_H
