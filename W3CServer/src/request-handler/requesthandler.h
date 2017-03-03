#ifndef REQUESTHANDLER_H
#define REQUESTHANDLER_H

#include <QObject>
#include <QWebSocket>
#include "vissrequest.h"
#include <QSharedPointer>
#include "messaging/websocketwrapper.h"
#include "VSSSignalinterface/vsssignalinterface.h"

class RequestHandler : public QObject
{
    Q_OBJECT

public:
    explicit RequestHandler(QObject *parent, QSharedPointer<VSSSignalInterface> signalInterface, VISSRequest* vissrequest = 0, WebSocketWrapper* client = 0
                           ):QObject(parent),
        m_pVissrequest(vissrequest), m_pClient(client), m_pSignalInterface(signalInterface) {}
    static QSharedPointer<RequestHandler> makeRequestHandler(const QString &message, WebSocketWrapper* aClient, QSharedPointer<VSSSignalInterface> signalInterface);
    virtual ~RequestHandler();

    virtual void processRequest()=0;

signals:

public slots:
protected:
    VISSRequest* m_pVissrequest;
    WebSocketWrapper* m_pClient;
    QSharedPointer<VSSSignalInterface> m_pSignalInterface;

};

#endif // REQUESTHANDLER_H
