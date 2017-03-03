#include "vsssignalinterfaceimpl.h"
#include <QJsonObject>
#include <QString>
#include <QMutexLocker>
#include <QPointer>

VSSSignalInterfaceImpl::VSSSignalInterfaceImpl()
{
    m_rpm = "3000";
    m_speed = "70";
}

QString VSSSignalInterfaceImpl::getSignalValue(const QString& path)
{
    QMutex mutex;
    QMutexLocker locker(&mutex);

    QString result = "not implemented";

    if(path == "vehicle.engine.rpm")
    {

        result = m_rpm;

    } else if (path == "vehicle.engine.speed")
    {

        result = m_speed;

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
