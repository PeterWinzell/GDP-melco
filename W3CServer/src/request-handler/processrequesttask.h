#ifndef PROCESSREQUESTTASK_H
#define PROCESSREQUESTTASK_H
#include <QWebSocket>
#include <QRunnable>

class WebSocketWrapper;

class ProcessRequestTask : public QRunnable
{
public:
    ProcessRequestTask(WebSocketWrapper* wsw, QString message, bool debug = false);
    void run();
private:
    WebSocketWrapper* m_pClient;
    bool m_debug;
    QString m_jsonRequestMessage;
};

#endif // PROCESSREQUESTTASK_H
