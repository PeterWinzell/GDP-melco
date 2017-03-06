#include "vsssignalinterfaceimpl.h"
#include <QJsonObject>
#include <QString>
#include <QMutex>
#include <QJsonDocument>
#include <QFile>
#include <QDebug>

VSSSignalInterfaceImpl::VSSSignalInterfaceImpl()
{
    rpm = "3000";
    speed = "70";

    QJsonDocument jsonDoc = loadJson("/home/vagrant/workspace/GDP-melco/W3CServer/vss_rel_1.json");

}

QJsonDocument VSSSignalInterfaceImpl::loadJson(const QString &fileName) {
    QFile jsonFile(fileName);

    QObjectList childs = jsonFile.children();

    if (jsonFile.exists())
    {
        jsonFile.open(QFile::ReadOnly);
    }
    else
    {
        qWarning() << "File " << fileName << " not found!";
    }

    QJsonDocument doc = QJsonDocument().fromJson(jsonFile.readAll());

    QJsonObject obj = doc.object();

    QJsonObject::const_iterator iter = obj.begin();

    for (QJsonObject::const_iterator iter = obj.begin(); iter != obj.end(); iter++) {
       QJsonValue val =  *iter;
    }


    return doc;
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
