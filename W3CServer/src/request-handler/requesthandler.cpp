#include "requesthandler.h"
#include "gethandler.h"

//factory method for requestHandler
RequestHandler* RequestHandler::makeRequestHandler(QString message){
    return new GetHandler();
}

