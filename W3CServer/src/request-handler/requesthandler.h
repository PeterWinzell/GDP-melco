#ifndef REQUESTHANDLER_H
#define REQUESTHANDLER_H

#include <QObject>
#include <QWebSocket>
#include "vissrequest.h"
#include <QSharedPointer>
#include "messaging/websocketwrapper.h"

class RequestHandler : public QObject
{
    Q_OBJECT

public:
    explicit RequestHandler(QObject *parent = 0,VISSRequest* vissrequest = 0, WebSocketWrapper* client = 0):QObject(parent),
        p_vissrequest(vissrequest), m_pClient(client) {}
    static QSharedPointer<RequestHandler> makeRequestHandler(const QString &message, WebSocketWrapper* aClient);
    virtual void processRequest()=0;

signals:

public slots:
protected:
    VISSRequest* p_vissrequest;
    WebSocketWrapper*  m_pClient;

};

#endif // REQUESTHANDLER_H
