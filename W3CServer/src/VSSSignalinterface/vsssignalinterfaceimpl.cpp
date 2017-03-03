#include "vsssignalinterfaceimpl.h"
#include <QJsonObject>
#include <QString>
#include <QMutex>

VSSSignalInterfaceImpl::VSSSignalInterfaceImpl()
{
    rpm = "3000";
    speed = "70";
}

QString VSSSignalInterfaceImpl::getSignalValue(const QString& path)
{
    QString result = "not implemented";

    if(path == "vehicle.engine.rpm")
    {
        mutex.lock();
        result = rpm;
        mutex.unlock();

    } else if (path == "vehicle.engine.speed")
    {
        mutex.lock();
        result = speed;
        mutex.unlock();
    }

    return result;
}

qint8 VSSSignalInterfaceImpl::setSignalValue(const QString& path)
{
    return 0;
}

QJsonObject VSSSignalInterfaceImpl::getVSSNode(const QString& path)
{
    QJsonObject obj;
    return obj;
}

QJsonObject VSSSignalInterfaceImpl::getVSSTree(const QString& path)
{
    QJsonObject tree;
    return tree;
}
