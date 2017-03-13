#ifndef OPENDSHANDLER_H
#define OPENDSHANDLER_H

#include <QObject>
#include <QTcpSocket>

class OpenDSHandler : public QObject
{
    Q_OBJECT
public:
    explicit OpenDSHandler(QObject *parent = 0);

signals:

public slots:
    void connected();
    void disconnected();
    void bytesWritten(qint64 bytes);
    void readyRead();

private:
    QString readSubscribeMessage();
    QTcpSocket *m_Socket;
};

#endif // OPENDSHANDLER_H
