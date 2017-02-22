#ifndef REQUESTHANDLER_H
#define REQUESTHANDLER_H

#include <QObject>
#include <QWebSocket>
#include "vissrequest.h"

class RequestHandler : public QObject
{
    Q_OBJECT
public:
    explicit RequestHandler(QObject *parent = 0,VISSRequest* vissrequest = 0,QWebSocket* client = 0):QObject(parent),
        p_vissrequest(vissrequest),p_client(client) {}
    static std::shared_ptr<RequestHandler> makeRequestHandler(QString message,QWebSocket* aClient);
    virtual void processRequest()=0;
signals:

public slots:
protected:
    VISSRequest* p_vissrequest;
    QWebSocket*  p_client;
};

#endif // REQUESTHANDLER_H
