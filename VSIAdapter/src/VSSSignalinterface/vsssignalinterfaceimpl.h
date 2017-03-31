#ifndef VSSSIGNALINTERFACEIMPL_H
#define VSSSIGNALINTERFACEIMPL_H

extern "C" {
#include "vsi.h"
}

#include <QObject>
#include <QMutex>
#include <QMap>
#include "vsssignalinterface.h"



class VSSSignalInterfaceImpl : public VSSSignalInterface
{
public:

    VSSSignalInterfaceImpl(const QString &vssFile);
    ~VSSSignalInterfaceImpl();
    QString getSignalValue(const QString& path);
    qint8   setSignalValue(const QString& path, QVariant value);
    QJsonObject getVSSNode(const QString& path);
    QJsonObject getVSSTree(const QString& path);

    struct JsonNode
    {
        QJsonObject json;
        QString key;
        bool isBranch;
    };

    enum CarSignalType
    {
        RPM,
        Speed,
        GasPedal,
        BrakePedal,
        SteerAngle,
        Headlights,
        EngineRunning,
        CurrentFuelConsumption,
        FuelTankMax,
        FuelTankActual,
        PositionLatitude,
        PositionLongitude,
        PositionAltitude,
        Orientation,
        Rise,
        AccelerationLateral,
        Rotation,
        AccelerationRotation,
        Acceleration,
        CarSignalType_NO_OF_ITEMS
    };
    Q_ENUM(CarSignalType)

public slots:
    void updateValue(CarSignalType type, QString value);


private:
    void loadJson(const QString& fileName);
    void getTreeNodes(QJsonObject& tree, QStringList& path, QVector<JsonNode>& nodes);
    void removeAllKeysButOne(QJsonObject &json, const QString &filter);
    void removeOne(QJsonObject &json, const QString &filter);
    void createJsonVssTree(QVector<JsonNode>& nodes, QJsonObject &json);

private:
    QString m_rpm;
    QString m_speed;
    QMutex m_mutex;

    QJsonObject m_vssTree;
    QJsonObject m_vssTreeNode;

    QMap<CarSignalType, signal_t> SignalLookup;

    domain_t m_domainId;
    signal_t m_signalId;
    vsi_result m_result;
};

#endif // VSSSIGNALINTERFACEIMPL_H
