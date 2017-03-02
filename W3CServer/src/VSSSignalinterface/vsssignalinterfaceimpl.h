#ifndef VSSSIGNALINTERFACEIMPL_H
#define VSSSIGNALINTERFACEIMPL_H

#include <QObject>
#include "vsssignalinterface.h"
#include <QMutex>

class VSSSignalInterfaceImpl : public VSSSignalInterface
{
public:
    VSSSignalInterfaceImpl();
    QString getSignalValue(QString path);
    qint8   setSignalValue(QString path);
    QJsonObject getVSSNode(QString path);
    QJsonObject getVSSTree(QString path);

private:
    QString rpm;
    QString speed;
    QMutex mutex;
};

#endif // VSSSIGNALINTERFACEIMPL_H
