#include "requesthandler.h"
#include "gethandler.h"
#include "sethandler.h"
#include "subscribehandler.h"
#include "unsubscribehandler.h"
#include "unsubscribeallhandler.h"
#include "getvsshandler.h"
#include "authorizationhandler.h"
#include "jsonrequestparser.h"
#include "vissrequest.h"

//factory method for requestHandler
QSharedPointer<RequestHandler> RequestHandler::makeRequestHandler(const QString& message, WebSocketWrapper* client)
{
    JSONRequestParser jsonParser(message);
    VISSRequest* parsedRequest = jsonParser.getRequest();

    QSharedPointer<RequestHandler> handler;


    switch(parsedRequest->getAction())
    {
        case GET:
            handler = QSharedPointer<GetHandler>(new GetHandler(nullptr,parsedRequest,client));
            break;
        case SET:
            handler = QSharedPointer<SetHandler>(new SetHandler(nullptr,parsedRequest,client));
            break;
        case SUBSCRIBE:
            handler = QSharedPointer<SubscribeHandler>(new SubscribeHandler(nullptr,parsedRequest,client));
            break;
        case UNSUBSCRIBE:
            handler = QSharedPointer<UnsubscribeHandler>(new UnsubscribeHandler(nullptr,parsedRequest,client));
            break;
        case UNSUBSCRIBEALL:
            handler = QSharedPointer<UnsubscribeAllHandler>(new UnsubscribeAllHandler(nullptr,parsedRequest,client));
            break;
        case GETVSS:
            handler = QSharedPointer<GetVSSHandler>(new GetVSSHandler(nullptr,parsedRequest,client));
            break;
        case AUTHORIZE:
            handler = QSharedPointer<AuthorizationHandler>(new AuthorizationHandler(nullptr,parsedRequest,client));
            break;
        default:
            break;
    }

    return handler;

}
