#ifndef STATUSHANDLER_H
#define STATUSHANDLER_H

#include "requesthandler.h"
#include "vissrequest.h"
#include <QWebSocket>

class StatusHandler : public RequestHandler
{
public:
    explicit StatusHandler(QObject *parent, QSharedPointer<VSSSignalInterface> signalInterface, QSharedPointer<VISSRequest> vissrequest,WebSocketWrapper* client=0);
    virtual ~StatusHandler();
    void processRequest();
};

#endif // STATUSHANDLER_H
