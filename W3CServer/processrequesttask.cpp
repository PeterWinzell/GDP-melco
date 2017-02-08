#include "processrequesttask.h"
#include <QWebSocket>
#include <QUrl>

ProcessRequestTask::ProcessRequestTask(QWebSocket* client, QString message, bool debug):
    m_client(client),
    m_debug(debug),
    m_jsonRequestMessage(message)
{

}

void ProcessRequestTask::run(){
    if (m_debug)
        qDebug() << "processRequestTask is running " << m_client ->requestUrl().host() << m_jsonRequestMessage;
}
