#ifndef GETVSSTESTCASE_H
#define GETVSSTESTCASE_H

#include <QObject>
#include <TestCases/testcase.h>

class GetVSSTestCase : public TestCaseBase
{
    Q_OBJECT
public:
    GetVSSTestCase(QString testClientId, int& requestid, QObject *parent = NULL);
    void startTest(QWebSocket* webSocket);

signals:
    void finished(bool result);

public slots:
    void forceStop();
    void onTextMessageReceived(QString message);
};
#endif // GETVSSTESTCASE_H
