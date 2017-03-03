#ifndef GETHANDLER_H
#define GETHANDLER_H
#include "requesthandler.h"
#include "vissrequest.h"
#include <QWebSocket>

class GetHandler : public RequestHandler
{
public:
    explicit GetHandler(QObject *parent, QSharedPointer<VSSSignalInterface> signalInterface, VISSRequest* vissrequest=0,WebSocketWrapper* client=0);
    void processRequest();
};

#endif // GETHANDLER_H
