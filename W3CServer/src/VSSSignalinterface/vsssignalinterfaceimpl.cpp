#include "vsssignalinterfaceimpl.h"
#include <QJsonObject>
#include <QString>

VSSSignalInterfaceImpl::VSSSignalInterfaceImpl()
{
    m_rpm = "3000";
    m_speed = "70";
}

QString VSSSignalInterfaceImpl::getSignalValue(QString path)
{
    if(path.compare("car.engine.rpm"))
    {
        m_mutex.lock();
        return m_rpm;
        m_mutex.unlock();

    } else if (path.compare("car.engine.speed"))
    {
        m_mutex.lock();
        return m_speed;
        m_mutex.unlock();
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
