#include <memory>
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
std::shared_ptr<RequestHandler> RequestHandler::makeRequestHandler(QString message,QWebSocket* client)
{
    JSONRequestParser jsonParser(message);
    VISSRequest* parsedRequest = jsonParser.getRequest();

   std::shared_ptr<RequestHandler> handler = nullptr;


    switch(parsedRequest->getAction())
    {
        case GET:
            handler = std::make_shared<GetHandler>(nullptr,parsedRequest,client);
            break;
        case SET:
            handler = std::make_shared<SetHandler>(nullptr,parsedRequest,client);
            break;
        case SUBSCRIBE:
            handler = std::make_shared<SubscribeHandler>(nullptr,parsedRequest,client);
            break;
        case UNSUBSCRIBE:
            handler = std::make_shared<UnsubscribeHandler>(nullptr,parsedRequest,client);
            break;
        case UNSUBSCRIBEALL:
            handler = std::make_shared<UnsubscribeAllHandler>(nullptr,parsedRequest,client);
            break;
        case GETVSS:
            handler = std::make_shared<GetVSSHandler>(nullptr,parsedRequest,client);
            break;
        case AUTHORIZE:
            handler = std::make_shared<AuthorizationHandler>(nullptr,parsedRequest,client);
            break;
        default:
            break;
    }

    return handler;

}
