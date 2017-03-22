#include "requesthandler.h"
#include "gethandler.h"
#include "sethandler.h"
#include "subscribehandler.h"
#include "unsubscribehandler.h"
#include "unsubscribeallhandler.h"
#include "getvsshandler.h"
#include "authorizationhandler.h"
#include "statushandler.h"
#include "jsonrequestparser.h"
#include "vissrequest.h"

//factory method for requestHandler
QSharedPointer<RequestHandler> RequestHandler::makeRequestHandler(const QString& message, WebSocketWrapper* client, QSharedPointer<VSSSignalInterface> signalInterface)
{
    JSONRequestParser jsonParser;
    QSharedPointer<VISSRequest> parsedRequest = jsonParser.parseJson(message);

    QSharedPointer<RequestHandler> handler;

    switch(parsedRequest->getAction())
    {
        case GET:
            handler = QSharedPointer<GetHandler>(new GetHandler(nullptr, signalInterface, parsedRequest,client));
            break;
        case SET:
            handler = QSharedPointer<SetHandler>(new SetHandler(nullptr,signalInterface, parsedRequest,client));
            break;
        case SUBSCRIBE:
            handler = QSharedPointer<SubscribeHandler>(new SubscribeHandler(nullptr,signalInterface, parsedRequest,client));
            break;
        case UNSUBSCRIBE:
            handler = QSharedPointer<UnsubscribeHandler>(new UnsubscribeHandler(nullptr, signalInterface, parsedRequest,client));
            break;
        case UNSUBSCRIBEALL:
            handler = QSharedPointer<UnsubscribeAllHandler>(new UnsubscribeAllHandler(nullptr,signalInterface, parsedRequest,client));
            break;
        case GETVSS:
            handler = QSharedPointer<GetVSSHandler>(new GetVSSHandler(nullptr,signalInterface, parsedRequest,client));
            break;
        case AUTHORIZE:
            handler = QSharedPointer<AuthorizationHandler>(new AuthorizationHandler(nullptr, signalInterface, parsedRequest,client));
            break;
        case STATUS:
            handler = QSharedPointer<StatusHandler>(new StatusHandler(nullptr, signalInterface, parsedRequest,client));
            break;
        default:
            break;
    }

    return handler;

}

RequestHandler::~RequestHandler()
{
    if (m_pClient != 0)
    {
        delete m_pClient;
    }

    //if (m_pVissrequest != 0)
    //{
    //    delete m_pVissrequest;
    //}
}
