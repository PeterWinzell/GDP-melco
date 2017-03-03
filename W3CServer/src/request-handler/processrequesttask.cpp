#include "processrequesttask.h"
#include "requesthandler.h"
#include <QWebSocket>
#include <QUrl>
#include "messaging/websocketwrapper.h"

ProcessRequestTask::ProcessRequestTask(WebSocketWrapper* wsw, QSharedPointer<VSSSignalInterface> signalInterface, QString message, bool debug):
    m_pClient(wsw),
    m_debug(debug),
    m_jsonRequestMessage(message),
    m_pSignalInterface(signalInterface)
{

}

void ProcessRequestTask::run()
{
    if (m_debug)
    {
        qDebug() << "processRequestTask is running " << m_pClient->getSocket()->requestUrl().host() << m_jsonRequestMessage;
    }

    auto aHandler = RequestHandler::makeRequestHandler(m_jsonRequestMessage, m_pClient, m_pSignalInterface);
    if(aHandler)
    {
        //blocking
        aHandler->processRequest();
    }
}
