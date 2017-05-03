#include <QWebSocket>
#include <QUrl>
#include "processrequesttask.h"
#include "requesthandler.h"
#include "messaging/websocketwrapper.h"
#include "VSSSignalinterface/vsssignalinterface.h"

//DN DEBUG
#include <QThread>

ProcessRequestTask::ProcessRequestTask(WebSocketWrapper* wsw, QSharedPointer<VSSSignalInterface> signalInterface, const QString &message, bool debug):
    m_pClient(wsw),
    m_debug(debug),
    m_jsonRequestMessage(message),
    m_pSignalInterface(signalInterface)
{
    TRACE("Server", "< ProcessRequestTask > created.");

    //DN DEBUG
    qDebug() << "ProcessRequestTask::ProcessRequestTask : task= " << QThread::currentThread();
    qDebug() << "ProcessRequestTask::ProcessRequestTask : socket thread 1= " << (m_pClient->getSocket())->thread();
 //   (m_pClient->getSocket())->moveToThread(nullptr);
    qDebug() << "ProcessRequestTask::ProcessRequestTask : socket thread 2= " << (m_pClient->getSocket())->thread();
}

ProcessRequestTask::~ProcessRequestTask()
{
    TRACE("Server", "< ProcessRequestTask > destroyed.");
}

void ProcessRequestTask::run()
{
    if (m_debug)
    {
        //qDebug() << "processRequestTask is running " << m_pClient->getSocket()->requestUrl().host() << m_jsonRequestMessage;
    }

    //DN DEBUG
    qDebug() << "ProcessRequestTask::run : task= " << QThread::currentThread();
    qDebug() << "ProcessRequestTask::run : socket thread 1= " << (m_pClient->getSocket())->thread();
 //   (m_pClient->getSocket())->moveToThread(QThread::currentThread());
    qDebug() << "ProcessRequestTask::run : socket thread 2= " << (m_pClient->getSocket())->thread();

    auto aHandler = RequestHandler::makeRequestHandler(m_jsonRequestMessage, m_pClient, m_pSignalInterface);
    if(aHandler)
    {
        //blocking
        aHandler->processRequest();
    }
}
