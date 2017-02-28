#ifndef VSSSIGNALINTERFACE_H
#define VSSSIGNALINTERFACE_H


class VSSSignalInterface
{
public:
    virtual QString getSignalValue(QString path) = 0;
    virtual qint8   setSignalValue(QString path) = 0;
    virtual QJsonObject getVSSNode(QString path) = 0;
    virtual QJsonObject getVSSTree(QString path) = 0;
};

#endif // VSSSIGNALINTERFACE_H
