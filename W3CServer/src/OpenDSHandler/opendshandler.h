#ifndef OPENDSHANDLER_H
#define OPENDSHANDLER_H

#include <QObject>

class OpenDSHandler : public QObject
{
    Q_OBJECT
public:
    explicit OpenDSHandler(QObject *parent = 0);
private:
    QString readSubscribeMessage();
signals:

public slots:
};

#endif // OPENDSHANDLER_H
