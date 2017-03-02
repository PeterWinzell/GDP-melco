#include "vsssignalinterfaceimpl.h"
#include <QJsonObject>
#include <QString>
#include <QMutex>

VSSSignalInterfaceImpl::VSSSignalInterfaceImpl()
{
    rpm = "3000";
    speed = "70";
}

QString VSSSignalInterfaceImpl::getSignalValue(QString path)
{
    if(path.compare("car.engine.rpm"))
    {
        mutex.lock();
        return rpm;
        mutex.unlock();

    } else if (path.compare("car.engine.speed"))
    {
        mutex.lock();
        return speed;
        mutex.unlock();
    } else
    {
        return "not implemented";
    }

}

qint8 VSSSignalInterfaceImpl::setSignalValue(QString path)
{
    return 0;
}

QJsonObject VSSSignalInterfaceImpl::getVSSNode(QString path)
{
    QJsonObject obj;
    return obj;
}

QJsonObject VSSSignalInterfaceImpl::getVSSTree(QString path)
{
    QJsonObject tree;
    return tree;
}
