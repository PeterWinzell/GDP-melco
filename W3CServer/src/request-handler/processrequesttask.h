#ifndef PROCESSREQUESTTASK_H
#define PROCESSREQUESTTASK_H
#include <QWebSocket>
#include <QRunnable>

class WebSocketWrapper;
class VSSSignalInterface;

class ProcessRequestTask : public QRunnable
{
public:
    ProcessRequestTask(WebSocketWrapper* wsw, QSharedPointer<VSSSignalInterface> signalInterface, const QString &message, bool debug = false);
    virtual ~ProcessRequestTask();
    void run();
private:
    WebSocketWrapper* m_pClient;
    bool m_debug;
    QString m_jsonRequestMessage;
    QSharedPointer<VSSSignalInterface> m_pSignalInterface;
};

#endif // PROCESSREQUESTTASK_H
