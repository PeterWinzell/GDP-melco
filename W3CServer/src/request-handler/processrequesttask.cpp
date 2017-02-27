#include  <memory>
#include "processrequesttask.h"
#include "requesthandler.h"
#include <QWebSocket>
#include <QUrl>

ProcessRequestTask::ProcessRequestTask(QWebSocket* client, QString message, bool debug):
    p_client(client),
    m_debug(debug),
    m_jsonRequestMessage(message)
{

}

void ProcessRequestTask::run(){
    if (m_debug)
        qDebug() << "processRequestTask is running " << p_client ->requestUrl().host() << m_jsonRequestMessage;

    auto aHandler = RequestHandler::makeRequestHandler(m_jsonRequestMessage,p_client);
    if(aHandler)
    {
        //blocking
        aHandler -> processRequest();
    }
}
