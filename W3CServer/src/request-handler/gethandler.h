#ifndef GETHANDLER_H
#define GETHANDLER_H
#include "gethandler.h"
#include "requesthandler.h"
class GetHandler : public RequestHandler
{
public:
    explicit GetHandler(QObject *parent = 0);
    void processRequest();
};

#endif // GETHANDLER_H
