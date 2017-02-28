#include "gethandler.h"
#include <QDebug>


GetHandler::GetHandler(QObject* parent,VISSRequest* vissrequest,QWebSocket *client):
    RequestHandler(parent,vissrequest,client)
{
}

void GetHandler::processRequest()
{
    qDebug() << " processing get handler requests";

}
