#ifndef VSSSIGNALINTERFACEIMPL_H
#define VSSSIGNALINTERFACEIMPL_H

#include <QObject>
#include <QMutex>
#include "vsssignalinterface.h"

class VSSSignalInterfaceImpl : public VSSSignalInterface
{
public:
    VSSSignalInterfaceImpl();
    QString getSignalValue(QString path);
    qint8   setSignalValue(QString path);
    QJsonObject getVSSNode(QString path);
    QJsonObject getVSSTree(QString path);

private:
    QString m_rpm;
    QString m_speed;
    QMutex m_mutex;
};

#endif // VSSSIGNALINTERFACEIMPL_H
