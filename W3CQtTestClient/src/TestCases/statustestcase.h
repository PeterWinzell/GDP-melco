#ifndef STATUSTESTCASE_H
#define STATUSTESTCASE_H

#include <QObject>
#include <TestCases/testcasebase.h>

class StatusTestCase : public TestCaseBase
{
    Q_OBJECT
public:
    StatusTestCase(QString testClientId, int& requestid, QObject *parent = NULL);
    void startTest(QWebSocket* webSocket);

signals:
    void finished(bool result);

public slots:
    void forceStop();
    void onTextMessageReceived(QString message);
};

#endif // STATUSTESTCASE_H
