#include "requesthandler.h"
#include "gethandler.h"
#include "authorizationhandler.h"

//factory method for requestHandler
RequestHandler* RequestHandler::makeRequestHandler(QString message){
    return new AuthorizationHandler();
}

