#ifndef OPENDSHANDLER_H
#define OPENDSHANDLER_H

#include <VSSSignalinterface/vsssignalinterfaceimpl.h>
#include <QObject>
#include <QTcpSocket>


class OpenDSHandler : public QObject
{
    Q_OBJECT
public:
    explicit OpenDSHandler(QObject *parent = 0);

signals:
    void valueChanged(VSSSignalInterfaceImpl::CarSignalType type, QString value);

public slots:
    void connected();
    void disconnected();
    void bytesWritten(qint64 bytes);
    void readyRead();
    void socketError(QAbstractSocket::SocketError error);

private:
    void connectToOpenDS();
    QByteArray getSubscribeMessage();
    QByteArray getSetMessage(VSSSignalInterfaceImpl::CarSignalType signal, QString value);
    void reconnect();
    void delay(int delay);
    void xmlParser(QString xmlData);

    QTcpSocket *m_Socket;
    QString m_server_ip;
    int m_server_port;
    int m_delay_sec;
};

#endif // OPENDSHANDLER_H
