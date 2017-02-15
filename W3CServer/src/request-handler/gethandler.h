#ifndef GETHANDLER_H
#define GETHANDLER_H
#include "gethandler.h"
#include "requesthandler.h"
class GetHandler : public RequestHandler
{
public:
    GetHandler();
    void processRequest();
};

#endif // GETHANDLER_H
