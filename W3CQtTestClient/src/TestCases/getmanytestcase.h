#ifndef GETMANYTESTCASE_H
#define GETMANYTESTCASE_H

#include <QObject>
#include <TestCases/testcase.h>

class GetManyTestCase : public TestCaseBase
{
    Q_OBJECT
public:
    GetManyTestCase(QString testClientId, int& requestid, QObject *parent = NULL);
    void startTest(QWebSocket* webSocket);

signals:
    void finished(bool result);

public slots:
    void forceStop();
    void onTextMessageReceived(QString message);
};

#endif // GETMANYTESTCASE_H
