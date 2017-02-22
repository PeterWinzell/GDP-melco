#ifndef PROCESSREQUESTTASK_H
#define PROCESSREQUESTTASK_H
#include <QWebSocket>
#include <QRunnable>

class ProcessRequestTask : public QRunnable
{
public:
    ProcessRequestTask(QWebSocket* client,QString message,bool debug=false);
    void run();
private:
    QWebSocket* p_client;
    bool m_debug;
    QString m_jsonRequestMessage;
};

#endif // PROCESSREQUESTTASK_H
