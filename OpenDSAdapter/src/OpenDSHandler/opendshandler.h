#ifndef OPENDSHANDLER_H
#define OPENDSHANDLER_H

#include <QObject>
#include <QTcpSocket>
#include <QObject>
#include <QMutex>
#include <QMap>
#include <QHash>



class OpenDSHandler : public QObject
{
    Q_OBJECT
public:
    explicit OpenDSHandler(QObject *parent = 0);
    //~OpenDSHandler();
    QVariant getSignalValue(const QString& path);
    bool setSignalValue(const QString& path, QVariant value);

    void updateSetSignalValues();

signals:
    void valueChanged(QString type, QString value);

public slots:
    void connected();
    void disconnected();
    void reconnect();
    void bytesWritten(qint64 bytes);
    void readyRead();
    void socketError(QAbstractSocket::SocketError error);
    void updateValue(QString signal, QString value);

private:
    QString m_rpm;
    QString m_speed;
    QMutex m_mutex;

    QHash<QString, QString> m_lookupGetProvider;  // (VSS path, OpenDS path)
    QHash<QString, QString> m_lookupSetProvider;  // (VSS path, OpenDS path)
    QHash<QString, QString> m_getValues;          // (OpenDS path, value)
    QHash<QString, QString> m_setValues;          // (OpenDS path, value)
    QHash<QString, QString> m_valueTypes;         // (OpenDS path, type)

    QByteArray getSubscribeMessage();
    QByteArray getSetMessage();

    void xmlParser(QString xmlData);

    QTcpSocket *m_Socket;
    QString m_server_ip;
    int m_server_port;
    int m_delay_sec;

};

#endif // OPENDSHANDLER_H
